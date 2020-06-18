#include <GL/glew.h>
#include "program.hh"
#include "vbo/teapot.hh"
#include "opengl_handling_error.hh"

namespace pogl {
    float width = 720;
    float height = 480;
    bool saved = false;
    float fpms = 60; //frame per mili second
    unsigned int display_program_index = 0;
    std::vector<shared_prog> programs;

    Program::Program() {}

    Program::Program(const GLuint &program_id) : pg_id(program_id) {}

    Program::Program(const GLuint &program_id, const GLuint &vs_id_, const GLuint &fs_id_)
            : pg_id(program_id), vs_id(vs_id_), fs_id(fs_id_) {}

    Program::~Program() {
        glDetachShader(pg_id, vs_id);
        glDetachShader(pg_id, fs_id);
        glDeleteShader(vs_id);
        glDeleteShader(fs_id);
        glDeleteProgram(pg_id);
    }

    GLuint Program::compile_shader(GLenum type, const std::string &source) {
        GLint compile_status = GL_TRUE;

        GLuint sid = glCreateShader(type);
        const char *src = source.c_str();

        glShaderSource(sid, 1, &src, 0);
        TEST_OPENGL_ERROR();

        glCompileShader(sid);
        TEST_OPENGL_ERROR();

        glGetShaderiv(sid, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
            GLsizei log_size;
            glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &log_size);

            GLchar *log_msg = (GLchar *) std::malloc(log_size + sizeof(GLchar));

            if (log_msg != 0) {
                glGetShaderInfoLog(sid, log_size, &log_size, log_msg);
                std::cerr << "SHADER " << sid << ": " << log_msg << std::endl;
                std::free(log_msg);
            }
            glDeleteShader(sid);
            return 0;
        }

        return sid;
    }

    shared_prog Program::make_program(const std::string &vertexShader, const std::string &fragmentShader) {
        GLuint vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
        if (vs == 0)
            return NULL;
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
        if (fs == 0)
            return NULL;

        GLuint program_id = glCreateProgram();
        TEST_OPENGL_ERROR();
        if (program_id == 0) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            return NULL;
        }

        glAttachShader(program_id, vs);
        glAttachShader(program_id, fs);

        shared_prog p = std::make_shared<Program>(program_id, vs, fs);

        glBindAttribLocation(program_id, 0, "position");
        TEST_OPENGL_ERROR();
        glBindAttribLocation(program_id, 1, "normal");
        TEST_OPENGL_ERROR();
        glBindAttribLocation(program_id, 2, "uv");
        TEST_OPENGL_ERROR();

        glLinkProgram(program_id);

        GLint link_status = GL_TRUE;
        glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

        if (link_status != GL_TRUE) {
            std::cout << "FAIL\n";
            GLint log_size;
            char *program_log;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);
            program_log = (char *) std::malloc(log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
            if (program_log != 0) {
                glGetProgramInfoLog(program_id, log_size, &log_size, program_log);
                std::cerr << "Program " << program_log << std::endl;
                std::free(program_log);
            }
            return NULL;
        }
        p->index = programs.size();
        programs.push_back(p);
        return p;
    }




    void Program::init_projection_view_matrices(const Vector3 &eye, const Vector3 &center, const Vector3 &up) {
        //projection matrix
        projection = matrix4::identity();
        float ratio = width / height;
        frustum(projection, -ratio, ratio, -1, 1, 5, 50000);
        GLuint pro_mat_id = glGetUniformLocation(pg_id, "projection");
        TEST_OPENGL_ERROR();
        glUniformMatrix4fv(pro_mat_id, 1, GL_FALSE, &projection.mat[0][0]);
        TEST_OPENGL_ERROR();

        //view matrix
        view = matrix4::identity();
        look_at(view, eye, center, up);
        GLuint view_mat_id = glGetUniformLocation(pg_id, "view");
        glUniformMatrix4fv(view_mat_id, 1, GL_FALSE, &view.mat[0][0]);
        TEST_OPENGL_ERROR();

        //window size
        int uniform_windowSize = glGetUniformLocation(pg_id, "windowsize");
        TEST_OPENGL_ERROR();
        glUniform2f(uniform_windowSize, width, height);
        TEST_OPENGL_ERROR();
    }


    GLuint Program::program_id() const {
        return this->pg_id;
    }

    void Program::use() {
        display_program_index = index;
        glUseProgram(this->pg_id);
    }

    shared_prog_obj Program::add_object(const OpenGLObject &obj,
                                        const matrix4 &transformation) {
        shared_prog_obj p_obj = std::make_shared<ProgramObject>(pg_id, obj);
        p_obj->set_transformation(transformation);
        objects.push_back(p_obj);
        return p_obj;
    }

    void Program::draw() {
        for (auto o: objects) {
            o->draw(this->pg_id);
        }
    }

    void Program::copy(shared_prog program) {
        this->projection = program->projection;
        this->view = program->view;
        for (auto o: program->objects)
            objects.push_back(o);
        GLuint pro_mat_id = glGetUniformLocation(pg_id, "projection");
        glUniformMatrix4fv(pro_mat_id, 1, GL_FALSE, &program->projection.mat[0][0]);
        TEST_OPENGL_ERROR();

        GLuint view_mat_id = glGetUniformLocation(pg_id, "view");
        glUniformMatrix4fv(view_mat_id, 1, GL_FALSE, &program->view.mat[0][0]);
        TEST_OPENGL_ERROR();

        //window size
        int uniform_windowSize = glGetUniformLocation(pg_id, "windowsize");
        glUniform2f(uniform_windowSize, width, height);
        TEST_OPENGL_ERROR();
    }

}
