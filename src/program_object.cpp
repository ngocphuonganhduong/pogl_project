#include <GL/glew.h>
#include "program_object.hh"
#include "opengl_handling_error.hh"

class value;
namespace pogl{
    void ProgramObject::draw(GLuint program_id){
        glBindVertexArray(obj.vao_id);
        TEST_OPENGL_ERROR();

        GLuint location = glGetUniformLocation(program_id, "transformation");
        TEST_OPENGL_ERROR();
        glUniformMatrix4fv(location, 1, GL_FALSE, &transformation.mat[0][0]);
        TEST_OPENGL_ERROR();

        location = glGetUniformLocation(program_id, "uniform_color");
        TEST_OPENGL_ERROR();
        glUniform3fv(location, 1, &uniform_color.val[0]);
        TEST_OPENGL_ERROR();

        for (auto text: textures) {
            location = glGetUniformLocation(program_id, text.first.c_str());
            TEST_OPENGL_ERROR();
            glUniform1i(location, text.second->unit);
            TEST_OPENGL_ERROR();
        }

        glDrawArrays(GL_TRIANGLES, 0, obj.vb_data_size);
    }

    void ProgramObject::add_texture(shared_text texture, std::string name) {
        textures.emplace(name, texture);
    }

    void ProgramObject::set_transformation(const matrix4& trans_) {
        transformation = trans_;
    }

//    void ProgramObject::set_normal(const matrix4 &transformation) {
//
//    }

}