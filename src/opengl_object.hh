#ifndef OPENGL_OBJECT_HH
#define OPENGL_OBJECT_HH

#include <GL/glew.h>
#include <vector>
#include "utils/matrix4.hh"
#include "texture.hh"

namespace pogl {

    struct OpenGLObject {
        OpenGLObject(const std::vector<GLfloat> &position, const std::vector<GLfloat> &normal,
                     const std::vector<GLfloat> &uv);

        GLuint vao_id{}; //vertex array object id/name
        GLuint vbo_id[3]; //vertex buffer object id/name
        GLuint vai[3]; //vertex attribute index
        GLuint vb_data_size = 0;

    private:
        static int max_vai; //max vertex attribute index;

    };
}


#endif //OPENGL_OBJECT_HH
