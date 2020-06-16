#include <GL/glew.h>
#include <iostream>
#include "opengl_handling_error.hh"

void test_opengl_error(const char *function_name, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL ERROR in " << function_name
                  << " at " << line <<
                  " : " << gluErrorString(err) << std::endl;
    }
}
