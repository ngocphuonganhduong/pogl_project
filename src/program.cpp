//
// Created by rimuru on 19/03/2020.
//

#include <GL/glew.h>

#include "program.hh"
#include "vbo/teapot.hh"

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

    void Program::add_data(const std::vector<GLfloat> &buffer_data, const char *var_name, GLint nb_components) {

        GLint location = glGetAttribLocation(pg_id, var_name);
        TEST_OPENGL_ERROR();
        GLuint buffer_id;

        if (location != -1) {
            glGenBuffers(1, &buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            TEST_OPENGL_ERROR();
            glBufferData(GL_ARRAY_BUFFER, buffer_data.size() * sizeof(float), buffer_data.data(), GL_STATIC_DRAW);
            TEST_OPENGL_ERROR();
            glVertexAttribPointer(location, nb_components, GL_FLOAT, GL_FALSE, 0, 0);
            TEST_OPENGL_ERROR();
            glEnableVertexAttribArray(location);
            TEST_OPENGL_ERROR();
        }

    }

    shared_obj Program::add_vbo(const std::vector<GLfloat> &vb_data, const char *var_name, GLint nb_components,
                                const matrix4 &transformation) {

        return add_vbo(vb_data, var_name, nb_components, transformation, Vector3(1));
    }

    shared_obj Program::add_vbo(const std::vector<GLfloat> &vb_data, const char *var_name, GLint nb_components,
                                const matrix4 &transformation, const Vector3 &uniform_color) {
        GLuint vbo_id;

        glGenVertexArrays(1, &vbo_id);
        TEST_OPENGL_ERROR();

        glBindVertexArray(vbo_id);
        TEST_OPENGL_ERROR();

        shared_obj obj = std::make_shared<Object>(pg_id, vbo_id, vb_data, transformation, uniform_color);
        this->objects.push_back(obj);

        glBindVertexArray(vbo_id);
        TEST_OPENGL_ERROR();

        add_data(vb_data, var_name, nb_components);
        return obj;
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
            return nullptr;
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
        if (fs == 0)
            return nullptr;

        GLuint program_id = glCreateProgram();
        TEST_OPENGL_ERROR();
        if (program_id == 0) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            return nullptr;
        }

        glAttachShader(program_id, vs);
        glAttachShader(program_id, fs);

        shared_prog p = std::make_shared<Program>(program_id, vs, fs);


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
        }
        p->isReady = link_status == GL_TRUE;
        p->index = programs.size();
        programs.push_back(p);
        return p;
    }


    bool Program::is_ready() {
        return isReady;
    }


    void Program::init_projection_view_matrices(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                                                const char *pro_name, const char *view_name) {
        //projection matrix

        matrix4 projection = matrix4::identity();
        float ratio = width / height;
        frustum(projection, -ratio, ratio, -1, 1, 5, 50000);

        matrix4 view = matrix4::identity();
        look_at(view, eye, center, up);

        int uniform_windowSize = glGetUniformLocation(pg_id, "windowsize");

        TEST_OPENGL_ERROR();

        GLuint pro_mat_id = glGetUniformLocation(pg_id, pro_name);
        TEST_OPENGL_ERROR();
        glUniformMatrix4fv(pro_mat_id, 1, GL_FALSE, &projection.mat[0][0]);
        TEST_OPENGL_ERROR();

        GLuint view_mat_id = glGetUniformLocation(pg_id, view_name);
        glUniformMatrix4fv(view_mat_id, 1, GL_FALSE, &view.mat[0][0]);
        TEST_OPENGL_ERROR();

        glUniform2f(uniform_windowSize, width, height);
        TEST_OPENGL_ERROR();
    }

    void Program::add_texture(shared_text texture) {
        GLuint texture_id;
        //      GLint location;

        glGenTextures(1, &texture_id);
        TEST_OPENGL_ERROR();
        glActiveTexture(GL_TEXTURE0 + texture->unit);
        TEST_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->sx, texture->sy, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texture->img_data);
        TEST_OPENGL_ERROR();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        TEST_OPENGL_ERROR();
    }


    GLuint Program::program_id() const {
        return this->pg_id;
    }

    void Program::use() {
        display_program_index = index;
        glUseProgram(this->pg_id);
    }


}
