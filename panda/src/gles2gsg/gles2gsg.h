/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file gles2gsg.h
 * @author rdb
 * @date 2009-06-14
 */

#ifndef GLES2GSG_H
#define GLES2GSG_H

// This header file compiles a GSG for the limited subset of OpenGL that is
// OpenGL ES 2.

#include "pandabase.h"
#include "config_gles2gsg.h"

#define GLP(name) gl##name

#ifndef STDFLOAT_DOUBLE
#define GLPf(name) gl ## name ## f
#define GLPfv(name) gl ## name ## fv
#else  // STDFLOAT_DOUBLE
#define GLPf(name) gl ## name ## d
#define GLPfv(name) gl ## name ## dv
#endif  // STDFLOAT_DOUBLE

#define CLP(name) GLES2##name
#define GLPREFIX_QUOTED "gl"
#define CLASSPREFIX_QUOTED "GLES2"
#define GLSYSTEM_NAME "OpenGL ES 2"
#define CONFIGOBJ config_gles2gsg
#define GLCAT gles2gsg_cat
#define EXPCL_GL EXPCL_PANDAGLES2
#define EXPTP_GL EXPTP_PANDAGLES2
#ifdef OPENGLES_1
  #error OPENGLES_1 should not be defined!
#endif
#ifndef OPENGLES
  #define OPENGLES
#endif
#ifndef OPENGLES_2
  #define OPENGLES_2
#endif

// OpenGL ES 2 has no fixed-function pipeline.
#undef SUPPORT_FIXED_FUNCTION

#ifdef BUILD_IPHONE
  #include <OpenGLES/ES2/gl.h>
// #include <OpenGLESES2glext.h>
#else
  #include <GLES2/gl2.h>
// #include <GLES2gl2ext.h>
#endif

#include "panda_esgl2ext.h"

// This helps to keep the source clean of hundreds of ifdefs.
typedef char GLchar;
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER
#define GL_RENDERBUFFER_RED_SIZE_EXT GL_RENDERBUFFER_RED_SIZE
#define GL_RENDERBUFFER_GREEN_SIZE_EXT GL_RENDERBUFFER_GREEN_SIZE
#define GL_RENDERBUFFER_BLUE_SIZE_EXT GL_RENDERBUFFER_BLUE_SIZE
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT GL_RENDERBUFFER_ALPHA_SIZE
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT GL_RENDERBUFFER_DEPTH_SIZE
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT GL_RENDERBUFFER_STENCIL_SIZE
#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER_EXT GL_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER_EXT GL_FRAMEBUFFER
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT GL_FRAMEBUFFER_UNSUPPORTED
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT GL_FRAMEBUFFER_INCOMPLETE_FORMATS
#define GL_DEPTH_ATTACHMENT_EXT GL_DEPTH_ATTACHMENT
#define GL_STENCIL_ATTACHMENT_EXT GL_STENCIL_ATTACHMENT
#define GL_DEPTH_STENCIL GL_DEPTH_STENCIL_OES
#define GL_DEPTH_STENCIL_EXT GL_DEPTH_STENCIL_OES
#define GL_UNSIGNED_INT_24_8_EXT GL_UNSIGNED_INT_24_8_OES
#define GL_DEPTH24_STENCIL8_EXT GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_OES
#define GL_MAX_3D_TEXTURE_SIZE GL_MAX_3D_TEXTURE_SIZE_OES
#define GL_SAMPLER_3D GL_SAMPLER_3D_OES
#define GL_BGRA GL_BGRA_EXT
#define GL_RED GL_RED_EXT
#define GL_RG GL_RG_EXT
#define GL_R16F GL_R16F_EXT
#define GL_RG16F GL_RG16F_EXT
#define GL_RGB16F GL_RGB16F_EXT
#define GL_RGBA16F GL_RGBA16F_EXT
#define GL_RGB16F GL_RGB16F_EXT
#define GL_RGBA16F GL_RGBA16F_EXT
#define GL_RGB32F GL_RGB32F_EXT
#define GL_RGBA32F GL_RGBA32F_EXT
#define GL_SRGB_ALPHA GL_SRGB_ALPHA_EXT
#define GL_RGBA8 GL_RGBA8_OES
#define GL_R8 GL_R8_EXT
#define GL_RG8 GL_RG8_EXT
#define GL_ALPHA8 GL_ALPHA8_OES
#define GL_LUMINANCE8 GL_LUMINANCE8_OES
#define GL_LUMINANCE8_ALPHA8 GL_LUMINANCE8_ALPHA8_EXT
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_EXT
#define GL_R32F GL_R32F_EXT
#define GL_RG32F GL_RG32F_EXT
#define GL_RGB8 GL_RGB8_OES
#define GL_TEXTURE_COMPARE_FUNC_ARB GL_TEXTURE_COMPARE_FUNC_EXT
#define GL_TEXTURE_COMPARE_MODE_ARB GL_TEXTURE_COMPARE_MODE_EXT
#define GL_COMPARE_R_TO_TEXTURE_ARB GL_COMPARE_REF_TO_TEXTURE_EXT
#define GL_SAMPLER_2D_SHADOW GL_SAMPLER_2D_SHADOW_EXT
#define GL_MAX_DRAW_BUFFERS GL_MAX_DRAW_BUFFERS_NV
#define GL_SRC1_COLOR GL_SRC1_COLOR_EXT
#define GL_ONE_MINUS_SRC1_COLOR GL_ONE_MINUS_SRC1_COLOR_EXT
#define GL_SRC1_ALPHA GL_SRC1_ALPHA_EXT
#define GL_ONE_MINUS_SRC1_ALPHA GL_ONE_MINUS_SRC1_ALPHA_EXT
#define GL_LOWER_LEFT GL_LOWER_LEFT_EXT
#define GL_ZERO_TO_ONE GL_ZERO_TO_ONE_EXT

#define GL_DEBUG_OUTPUT_SYNCHRONOUS GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR
#define GL_DEBUG_TYPE_PERFORMANCE GL_DEBUG_TYPE_PERFORMANCE_KHR
#define GL_DEBUG_SEVERITY_HIGH GL_DEBUG_SEVERITY_HIGH_KHR
#define GL_DEBUG_SEVERITY_MEDIUM GL_DEBUG_SEVERITY_MEDIUM_KHR
#define GL_DEBUG_SEVERITY_LOW GL_DEBUG_SEVERITY_LOW_KHR
#define GL_DEBUG_SEVERITY_NOTIFICATION GL_DEBUG_SEVERITY_NOTIFICATION_KHR
#define GL_BUFFER GL_BUFFER_KHR
#define GL_SHADER GL_SHADER_KHR
#define GL_PROGRAM GL_PROGRAM_KHR
#define GL_DEBUG_OUTPUT GL_DEBUG_OUTPUT_KHR

// For GLES 3 compat - need a better solution for this
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x1
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x2
#define GL_UNIFORM_BARRIER_BIT 0x4
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x8
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x20
#define GL_COMMAND_BARRIER_BIT 0x40
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x80
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x1000
#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#define GL_HALF_FLOAT 0x140B
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_RGB10_A2 0x8059
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_NUM_EXTENSIONS 0x821D
#define GL_RG_INTEGER 0x8228
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_MAX_VERTEX_ATTRIB_STRIDE 0x82E5
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_COMPUTE_SHADER 0x91B9
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279

#undef SUPPORT_IMMEDIATE_MODE
#define APIENTRY
#define APIENTRYP *

#include "glstuff_src.h"

#endif  // GLES2GSG_H
