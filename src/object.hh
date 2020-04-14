//
// Created by rimuru on 12/04/2020.
//

#ifndef OBJECT_HH
#define OBJECT_HH


#include <GL/glew.h>
#include "utils/matrix4.hh"
#include "utils/image.hh"
#include "texture.hh"

namespace pogl {

    class Object {
    public:
        Object(const GLuint &program_id, const GLuint &vbo_id, const std::vector<GLfloat> &vb_data,
               const matrix4 &transformation, const Vector3 &color);

        Object(const GLuint &program_id, const GLuint &vbo_id, const std::vector<GLfloat> &vb_data,
               const matrix4 &transformation);

        void add_texture(shared_text texture);
        void draw();

        matrix4 transformation;
        Vector3 uniform_color;
    private:
        GLuint program_id;
        GLuint vbo_id;
        std::vector<GLfloat> vb_data;
        shared_text texture;
    };
    using shared_obj = std::shared_ptr<Object>;

}


#endif //OBJECT_HH
