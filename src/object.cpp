//
// Created by rimuru on 12/04/2020.
//

#include "program.hh"
#include "object.hh"


namespace pogl {
    void Object::draw() {
        //vbo: program_id, vbo_id, vb data, transformation matrix
        glBindVertexArray(vbo_id);
        TEST_OPENGL_ERROR();

        GLuint location = glGetUniformLocation(program_id, "transformation");
        TEST_OPENGL_ERROR();
        glUniformMatrix4fv(location, 1, GL_FALSE, &transformation.mat[0][0]);
        TEST_OPENGL_ERROR();

        location = glGetUniformLocation(program_id, "uniform_color");
        TEST_OPENGL_ERROR();
        glUniform3fv(location, 1, &uniform_color.val[0]);
        TEST_OPENGL_ERROR();

        location = glGetUniformLocation(program_id, "texture_sampler");
        TEST_OPENGL_ERROR();
        glUniform1i(location, texture->unit);
        TEST_OPENGL_ERROR();

        glDrawArrays(GL_TRIANGLES, 0, vb_data.size());

    }

    Object::Object(const GLuint &program_id_, const GLuint &vbo_id_, const std::vector<GLfloat> &vb_data_,
                   const matrix4 &transformation_, const Vector3 &color) : program_id(program_id_), vbo_id(vbo_id_),
                                                                           vb_data(vb_data_),
                                                                           transformation(transformation_),
                                                                           uniform_color(color) {}

    Object::Object(const GLuint &program_id_, const GLuint &vbo_id_, const std::vector<GLfloat> &vb_data_,
                   const matrix4 &transformation_) : program_id(program_id_), vbo_id(vbo_id_),
                                                     vb_data(vb_data_),
                                                     transformation(transformation_) {}

    void Object::add_texture(shared_text text) {
        texture = text;
    }

}
