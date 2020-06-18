#ifndef program_HH
#define program_HH

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include "opengl_object.hh"
#include "program_object.hh"

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

        shared_prog_obj add_object(const OpenGLObject &obj, const matrix4 &transformation);

        void init_projection_view_matrices(const Vector3 &eye, const Vector3 &center, const Vector3 &up);

        void copy(std::shared_ptr<Program> program);

        void use();

        void draw();

        GLuint program_id() const;

        std::vector<shared_prog_obj> objects;

    private:
        GLuint pg_id; //program id
        GLuint vs_id; //vertex shader id
        GLuint fs_id; //fragment shader id
        matrix4 view;
        matrix4 projection;
        unsigned int index = 0;
    };

    using shared_prog = std::shared_ptr<Program>;
    extern std::vector<shared_prog> programs;

    void use_program(unsigned int index);
}


#endif //program_HH
