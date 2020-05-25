//
// Created by rimuru on 19/03/2020.
//

#ifndef program_HH
#define program_HH

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include "object.hh"

#define TEST_OPENGL_ERROR2(name)                 \
  do {                                          \
    GLenum err = glGetError();                  \
    if (err != GL_NO_ERROR)                     \
        std::cerr << "OpenGL ERROR "            \
        << err << ": " << gluErrorString(err)   \
        << " at position: "                     \
        << name << std::endl;                   \
  } while(0)

#define TEST_OPENGL_ERROR()                                                             \
  do {                                    \
    GLenum err = glGetError();                                            \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR " << __LINE__<< ": " << gluErrorString(err) << std::endl;      \
  } while(0)


namespace pogl {
    extern float width;
    extern float height;
    extern bool saved;
    extern float fpms; //frame per mili second
    extern unsigned int display_program_index;

    class Program {
    public:
        Program(const GLuint &program_id);

        Program(const GLuint &program_id, const GLuint &vs_id, const GLuint &fs_id);

        Program();

        ~Program();

        static std::shared_ptr<Program> make_program(const std::string &vertexShader,
                                                     const std::string &fragmentShader);

        static GLuint compile_shader(GLenum type, const std::string &src);


        shared_obj add_vbo(const std::vector<GLfloat> &vertex_buffer_data,
                           const char *var_name, GLint nb_components, const matrix4 &transformation);

        shared_obj add_vbo(const std::vector<GLfloat> &vertex_buffer_data,
                           const char *var_name, GLint nb_components, const matrix4 &transformation,
                           const Vector3 &uniform_color);

        void add_data(const std::vector<GLfloat> &vertex_buffer_data,
                      const char *var_name, GLint nb_components);

        void add_texture(shared_text texture);

        bool create_frame_buffer(const char *name);

        void init_projection_view_matrices(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                                           const char *pro_name, const char *view_name);


        bool is_ready();

        void use();

        GLuint program_id() const;

        std::vector<shared_obj> objects;

    private:
        GLuint pg_id; //program id
        GLuint vs_id; //vertex shader id
        GLuint fs_id; //fragment shader id
        bool isReady = false;
        unsigned int index = 0;
    };

    using shared_prog = std::shared_ptr<Program>;
    extern std::vector<shared_prog> programs;

    void use_program(unsigned int index);
}


#endif //program_HH
