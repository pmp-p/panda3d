/**
 * @file pandagles2.cxx
 * @author rdb
 * @date 2009-06-08
 */

#include "pandagles2.h"

#define OPENGLES_2
#include "config_gles2gsg.h"


#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
    #if __EMSCRIPTEN__
        #include "config_webgldisplay.h"
        #include "webGLGraphicsPipe.h"

        #define init_libviewdisplay init_libwebgldisplay
        #define viewGraphicsPipe WebGLGraphicsPipe
    #endif //__EMSCRIPTEN__

    #if __ANDROID__
        #include "config_androiddisplay.h"
        #include "viewGraphicsPipe.h"
        #define init_libviewdisplay init_libandroiddisplay
        #define viewGraphicsPipe AndroidGraphicsPipe
    #endif //__ANDROID__
#else
    #include "config_egldisplay.h"
    #include "eglGraphicsPipe.h"

    #define init_libviewdisplay init_libegldisplay
    #define viewGraphicsPipe eglGraphicsPipe
#endif //defined(__ANDROID__) || defined(__EMSCRIPTEN__)


extern void init_libviewdisplay();

/**
 * Initializes the library.  This must be called at least once before any of
 * the functions or classes in this library can be used.  Normally it will be
 * called by the static initializers and need not be called explicitly, but
 * special cases exist.
 */

#if __EMSCRIPTEN__
static
#endif
void
init_libpandagles2() {
  init_libgles2gsg();
  init_libviewdisplay();
}

/**
 * Returns the TypeHandle index of the recommended graphics pipe type defined
 * by this module.
 */
int
get_pipe_type_pandagles2() {
  return viewGraphicsPipe::get_class_type().get_index();
}


