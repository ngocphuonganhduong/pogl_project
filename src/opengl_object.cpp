#include "opengl_object.hh"
#include "opengl_handling_error.hh"

namespace pogl {
    int OpenGLObject::max_vai = 0;

    OpenGLObject::OpenGLObject(const std::vector<GLfloat> &position, const std::vector<GLfloat> &normal,
                               const std::vector<GLfloat> &uv) {
        /** Allocate and bind vertex array object as current used object **/
        glGenVertexArrays(1, &vao_id);
        TEST_OPENGL_ERROR();
        glBindVertexArray(vao_id);
        TEST_OPENGL_ERROR();

        /** Allocate and bind vertex array object as current used object **/
        glGenBuffers(3, vbo_id);
        TEST_OPENGL_ERROR();

        //POSITION
        vb_data_size = position.size();
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(0);        /* Enable attribute index 0 as being used */
        TEST_OPENGL_ERROR();

        //NORMAL
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), normal.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(1);        /* Enable attribute index 0 as being used */
        TEST_OPENGL_ERROR();        //NORMAL

        //UV
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id[2]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float), uv.data(), GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(2);        /* Enable attribute index 0 as being used */
        TEST_OPENGL_ERROR();
    }


//    Object::Object(const std::vector<GLfloat> &vb_data_,
//                   const matrix4 &transformation_, const Vector3 &color) : vb_data(vb_data_),
//                                                                           transformation(transformation_),
//                                                                           uniform_color(color) {}


}
