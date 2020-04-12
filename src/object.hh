//
// Created by rimuru on 12/04/2020.
//

#ifndef OBJECT_HH
#define OBJECT_HH


#include <GL/glew.h>
#include "matrix4.hh"

namespace pogl {
    class Object {
    public:
        Object(const GLuint &program_id, const GLuint &vbo_id, const std::vector<GLfloat> &vb_data,
               const matrix4 &transformation, const Vector3 &color);

        Object(const GLuint &program_id, const GLuint &vbo_id, const std::vector<GLfloat> &vb_data,
               const matrix4 &transformation);

        void draw();

        matrix4 transformation;
        Vector3 uniform_color;
    private:
        GLuint program_id;
        GLuint vbo_id;
        std::vector<GLfloat> vb_data;
    };

}


#endif //OBJECT_HH
