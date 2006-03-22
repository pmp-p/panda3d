// Filename: glGraphicsBuffer_src.cxx
// Created by:  jyelon (15Jan06)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//

TypeHandle CLP(GraphicsBuffer)::_type_handle;


////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
CLP(GraphicsBuffer)::
CLP(GraphicsBuffer)(GraphicsPipe *pipe,
                    const string &name,
                    int x_size, int y_size, int flags,
                    GraphicsStateGuardian *gsg,
                    GraphicsOutput *host) :
  GraphicsBuffer(pipe, name, x_size, y_size, flags, gsg, host)
{
  // Since the FBO never gets flipped, we get screenshots from the
  // same buffer we draw into.
  _screenshot_buffer_type = _draw_buffer_type;

  // Initialize these.
  _fbo = 0;
  _rb_size_x = 0;
  _rb_size_y = 0;
  for (int i=0; i<SLOT_COUNT; i++) {
    _rb[i] = 0;
    _attached[i] = 0;
  }
  _attach_point[SLOT_depth]   = GL_DEPTH_ATTACHMENT_EXT;
  _attach_point[SLOT_stencil] = GL_STENCIL_ATTACHMENT_EXT;
  _attach_point[SLOT_color]   = GL_COLOR_ATTACHMENT0_EXT;

  _slot_format[SLOT_depth]   = GL_DEPTH_COMPONENT;
  _slot_format[SLOT_stencil] = GL_STENCIL_INDEX;
  _slot_format[SLOT_color]   = GL_RGBA;
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CLP(GraphicsBuffer)::
~CLP(GraphicsBuffer)() {
}
 
////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::begin_frame
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               before beginning rendering for a given frame.  It
//               should do whatever setup is required, and return true
//               if the frame should be rendered, or false if it
//               should be skipped.
////////////////////////////////////////////////////////////////////
bool glGraphicsBuffer::
begin_frame(FrameMode mode) {
  PStatTimer timer(_make_current_pcollector);
  if (!_is_valid) {
    return false;
  }

  if (!_host->begin_frame(FM_parasite)) {
    return false;
  }
  
  // Figure out the desired size of the buffer.
  if (mode == FM_render) {
    rebuild_bitplanes();
    clear_cube_map_selection();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::rebuild_bitplanes
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               to allocate/reallocate the fbo and all the associated
//               renderbuffers, just before rendering a frame.
////////////////////////////////////////////////////////////////////
void glGraphicsBuffer::
rebuild_bitplanes() {

  glGraphicsStateGuardian *glgsg;
  DCAST_INTO_R(glgsg, _gsg, false);

  // Bind the FBO

  if (_fbo == 0) {
    glgsg->_glGenFramebuffersEXT(1, &_fbo);
    if (_fbo == 0) {
      glgsg->report_my_gl_errors();
      return;
    }
  }
  glgsg->_glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);

  // Calculate bitplane size.  This can be larger than the buffer.

  if (_creation_flags & GraphicsPipe::BF_track_host_size) {
    if ((_host->get_x_size() != _x_size)||
        (_host->get_y_size() != _y_size)) {
      set_size_and_recalc(_host->get_x_size(),
                          _host->get_y_size());
    }
  }
  int desired_x = _x_size;
  int desired_y = _y_size;
  if (!glgsg->get_supports_tex_non_pow2()) {
    desired_x = Texture::up_to_power_2(desired_x);
    desired_y = Texture::up_to_power_2(desired_y);
  }

  // Scan the textures list and determine what should be attached.
  
  Texture *attach[SLOT_COUNT];
  for (int i=0; i<SLOT_COUNT; i++) {
    attach[i] = 0;
  }
  for (int i=0; i<count_textures(); i++) {
    if (get_rtm_mode(i) != RTM_bind_or_copy) {
      continue;
    }
    Texture *tex = get_texture(i);
    Texture::Format fmt = tex->get_format();

    // If it's a not a 2D texture or a cube map, punt it.
    if ((tex->get_texture_type() != Texture::TT_2d_texture)&&
        (tex->get_texture_type() != Texture::TT_cube_map)) {
      _rtm_mode[i] = RTM_copy_texture;
      continue;
    }
    
    // Identify right attachment point.
    
    int slot = SLOT_COUNT;
    if (fmt == Texture::F_depth_component) {
      slot = SLOT_depth;
    } else if (fmt == Texture::F_stencil_index) {
      slot = SLOT_stencil;
    } else if (fmt == Texture::F_rgba) {
      slot = SLOT_color;
    } else {
      _rtm_mode[i] = RTM_copy_texture;
      continue;
    }
    
    // If there's already a texture bound to this slot,
    // then punt this texture.  
    if (attach[slot]) {
      _rtm_mode[i] = RTM_copy_texture;
      continue;
    }
    
    // Assign the texture to this slot.
    attach[slot] = tex;
  }


  // For all slots, update the slot.
    
  for (int slot=0; slot<SLOT_COUNT; slot++) {
    Texture *tex = attach[slot];
    if (tex) {
      // If the texture is already bound to the slot, and it's
      // the right size, then no update of this slot is needed.
      if ((_tex[slot] == tex)&&
          (tex->get_x_size() == desired_x)&&
          (tex->get_y_size() == desired_y)) {
        continue;
      }
      
      // Bind the texture to the slot.
      tex->set_x_size(desired_x);
      tex->set_y_size(desired_y);
      TextureContext *tc = tex->prepare_now(get_prepared_objects(), this);
      nassertv(tc != (TextureContext *)NULL);
      CLP(TextureContext) *gtc = DCAST(CLP(TextureContext), tc);
      
      if (tex->get_texture_type() == Texture::TT_2d_texture) {
        glgsg->_glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, _attach_point[slot],
                                       GL_TEXTURE_2D, gtc->_index, 0);
      } else {
        glgsg->_glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, _attach_point[slot],
                                       GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, gtc->_index, 0);
      }
      _tex[slot] = tex;
      
      // If there was a renderbuffer bound to this slot, delete it.
      if (_rb[slot] != 0) {
        glgsg->_glDeleteRenderbuffers(1, &(_rb[slot]));
        _rb[slot] = 0;
      }
      
    } else {
      
      // If a renderbuffer is already attached to the slot, and it's
      // the right size, then no update of this slot is needed.
      if ((_rb[slot] != 0)&&
          (_rb_size_x == desired_x)&&
          (_rb_size_y == desired_y)) {
        continue;
      }
      
      // If there's no renderbuffer for this slot, create one.
      if (_rb[slot] == 0) {
        glgsg->_glGenRenderbuffers(1, &(_rb[slot]));
      }
      
      // Resize the renderbuffer appropriately.
      glgsg->_glBindRenderbuffer(GL_RENDERBUFFER_EXT, _rb[slot]);
      glgsg->_glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, _slot_format[slot],
                                       desired_x, desired_y);
      glgsg->_glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
      
      // Bind the renderbuffer to the slot.
      glgsg->_glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, _attach_point[slot],
                                        GL_RENDERBUFFER_EXT, _rb[slot]);
      
      // Toss any texture that was connected to the slot.
      _tex[slot] = 0;
    }
  }
  
  // These record the size of all nonzero renderbuffers.
  _rb_size_x = desired_x;
  _rb_size_y = desired_y;
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::generate_mipmaps
//       Access: Private
//  Description: This function will be called within the draw thread
//               after rendering is completed for a given frame.  
//               If we've just rendered into level zero of a mipmapped
//               texture, then all subsequent mipmap levels will now
//               be calculated.
////////////////////////////////////////////////////////////////////
void glGraphicsBuffer::
generate_mipmaps() {
  glGraphicsStateGuardian *glgsg;
  DCAST_INTO_R(glgsg, _gsg, false);

  for (int slot=0; slot<SLOT_COUNT; slot++) {
    Texture *tex = _tex[slot];
    if ((tex != 0) && (tex->uses_mipmaps())) {
      glgsg->_state._texture = 0;
      TextureContext *tc = tex->prepare_now(get_prepared_objects(), this);
      nassert(tc != (TextureContext *)NULL);
      CLP(TextureContext) *gtc = DCAST(CLP(TextureContext), tc);
      GLenum target = get_texture_target(tex->get_texture_type());
      GLP(BindTexture)(target, gtc->_index);
      glgsg->_glGenerateMipmap(target);
      GLP(BindTexture)(target, 0);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::end_frame
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               after rendering is completed for a given frame.  It
//               should do whatever finalization is required.
////////////////////////////////////////////////////////////////////
void glGraphicsBuffer::
end_frame(FrameMode mode) {
  end_frame_spam();
  nassertv(_gsg != (GraphicsStateGuardian *)NULL);

  if (mode == FM_render) {
    copy_to_textures();
  }

  // Unbind the FBO
  glgsg->_glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  
  if (mode == FM_render) {
    generate_mipmaps();
  }

  _host->end_frame(FM_parasite);

  if (mode == FM_render) {
    trigger_flip();
    if (_one_shot) {
      prepare_for_deletion();
    }
    clear_cube_map_selection();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::select_cube_map
//       Access: Public, Virtual
//  Description: Called internally when the window is in
//               render-to-a-texture mode and we are in the process of
//               rendering the six faces of a cube map.  This should
//               do whatever needs to be done to switch the buffer to
//               the indicated face.
////////////////////////////////////////////////////////////////////
void CLP(GraphicsBuffer)::
select_cube_map(int cube_map_index) {
  GLCAT.error() << "select_cube_map not implemented yet.\n";
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::open_buffer
//       Access: Protected, Virtual
//  Description: Opens the window right now.  Called from the window
//               thread.  Returns true if the window is successfully
//               opened, or false if there was a problem.
////////////////////////////////////////////////////////////////////
bool glGraphicsBuffer::
open_buffer() {

  // Check for support of relevant extensions.
  glGraphicsStateGuardian *glgsg;
  DCAST_INTO_R(glgsg, _gsg, false);
  if ((!glgsg->_supports_framebuffer_object)||
      (glgsg->_glDrawBuffers == 0)) {
    return false;
  }

  _is_valid = true;
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::close_buffer
//       Access: Protected, Virtual
//  Description: Closes the buffer right now.  Called from the window
//               thread.
////////////////////////////////////////////////////////////////////
void glGraphicsBuffer::
close_buffer() {

  // Get the glgsg.
  glGraphicsStateGuardian *glgsg;
  DCAST_INTO_R(glgsg, _gsg, false);
  
  // Delete the renderbuffers.
  for (int i=0; i<SLOT_COUNT; i++) {
    if (_rb[i] != 0) {
      glgsg->_glDeleteRenderbuffersEXT(1, &(_rb[i]));
      _rb[i] = 0;
    }
    _tex[i] = 0;
  }
  _rb_size_x = 0;
  _rb_size_y = 0;
  
  // Delete the FBO itself.
  nassertv(_fbo != 0, false);
  glgsg->_glDeleteFramebuffersEXT(1, &_fbo);
}

////////////////////////////////////////////////////////////////////
//     Function: glGraphicsBuffer::release_gsg
//       Access: Public, Virtual
//  Description: Releases the current GSG pointer, if it is currently
//               held, and resets the GSG to NULL.  The window will be
//               permanently unable to render; this is normally called
//               only just before destroying the window.  This should
//               only be called from within the draw thread.
////////////////////////////////////////////////////////////////////
void CLP(GraphicsBuffer)::
release_gsg() {
  GraphicsBuffer::release_gsg();
}

