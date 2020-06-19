#include <fstream>
#include "setup_shader.hh"
#include "frame_buffer.hh"
#include "init.hh"
#include "opengl_handling_error.hh"

namespace pogl {
    std::string load(const std::string &filename) {
        std::ifstream input_src_file(filename, std::ios::in);
        std::string line;
        std::string file_content = "";
        if (input_src_file.fail()) {
            std::cerr << "FAIL\n";
            return "";
        }
        while (getline(input_src_file, line)) {
            file_content = file_content + line + "\n";
        }
        file_content += '\0';
        input_src_file.close();
        return file_content;
    }

    bool setup_glitch(const Vector3 &eye, const Vector3 &center, const Vector3 &up, const OpenGLObject &rectangle) {
        FrameBuffer fbo;
        auto left_texture = fbo.bind_and_attach_to_texture(width, height);
        display();
        std::string vertex_src = load("../src/shaders/glitch/vertex.shd");
        std::string fragment_src = load("../src/shaders/glitch/fragment.shd");
        shared_prog prog2 = Program::make_program(vertex_src, fragment_src);
        if (prog2 == NULL)
            return false;
        prog2->use();
        prog2->init_projection_view_matrices(eye, center, up);

        matrix4 trans = matrix4::identity();
        trans.scaled(14, 14, 14);
        trans.translated(-3.2, -3.6, -50);
        auto obj = prog2->add_object(rectangle, trans);
        obj->add_texture(left_texture, "texture_sampler");
        return true;
    }

    bool setup_anaglyph(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                              const OpenGLObject &rectangle, shared_prog scene_program)
    {
        FrameBuffer fbo;
        auto left_texture = fbo.bind_and_attach_to_texture(width, height);
        display();

        FrameBuffer fbo2;
        auto right_texture = fbo.bind_and_attach_to_texture(width, height);
        matrix4 projection = matrix4::identity();
        float ratio = width / height;
        frustum(projection, -ratio + 0.04, ratio + 0.04, -1, 1, 5, 50000);
        GLuint pro_mat_id = glGetUniformLocation(scene_program->program_id(), "projection");
        glUniformMatrix4fv(pro_mat_id, 1, GL_FALSE, &projection.mat[0][0]);
        TEST_OPENGL_ERROR();
        display();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        std::string vertex_src = load("../src/shaders/anaglyph/vertex.shd");
        std::string fragment_src = load("../src/shaders/anaglyph/fragment.shd");
        shared_prog prog2 = Program::make_program(vertex_src, fragment_src);
        if (prog2 == NULL)
            return false;
        prog2->use();
        prog2->init_projection_view_matrices(eye, center, up);

        matrix4 trans = matrix4::identity();
        trans.scaled(14, 14, 14);
        trans.translated(-3.2, -3.6, -50);
        auto obj = prog2->add_object(rectangle, trans);
        obj->add_texture(left_texture, "texture_left");
        obj->add_texture(right_texture, "texture_right");
        return true;
    }

    bool setup_depth_of_field(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                              const OpenGLObject &rectangle, shared_prog scene_program) {
        FrameBuffer fbo1;
        auto scene_texture = fbo1.bind_and_attach_to_texture(width, height);
        display();

        std::string vertex_src2 = load("../src/shaders/depth_map/vertex.shd");
        std::string fragment_src2 = load("../src/shaders/depth_map/fragment.shd");
        shared_prog depth_map_program = Program::make_program(vertex_src2, fragment_src2);
        if (depth_map_program == NULL)
            return false;
        depth_map_program->use();
        depth_map_program->copy(scene_program);
        FrameBuffer fbo2;
        auto depth_texture = fbo2.bind_and_attach_to_texture(width, height);
        display();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        std::string vertex_src3 = load("../src/shaders/depth_of_field/vertex.shd");
        std::string fragment_src3 = load("../src/shaders/depth_of_field/fragment.shd");
        shared_prog prog3 = Program::make_program(vertex_src3, fragment_src3);
        if (prog3 == NULL)
            return false;
        prog3->use();
        prog3->init_projection_view_matrices(eye, center, up);
        matrix4 trans = matrix4::identity();
        trans.scaled(14, 14, 14);
        trans.translated(-3.2, -3.6, -50);
        auto obj = prog3->add_object(rectangle, trans);
        obj->add_texture(scene_texture, "scene_texture");
        obj->add_texture(depth_texture, "depth_texture");
        return true;
    }

    bool setup_swirl(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                     const OpenGLObject &rectangle, shared_prog scene_program){
        FrameBuffer fbo;
        auto scene_texture = fbo.bind_and_attach_to_texture(width, height);
        display();
        std::string vertex_src = load("../src/shaders/swirl/vertex.shd");
        std::string fragment_src = load("../src/shaders/swirl/fragment.shd");
        shared_prog swirl_program = Program::make_program(vertex_src, fragment_src);
        if (swirl_program == NULL)
            return false;
        swirl_program->use();
        swirl_program->init_projection_view_matrices(eye, center, up);

        matrix4 trans = matrix4::identity();
        trans.scaled(14, 14, 14);
        trans.translated(-3.2, -3.6, -50);
        auto obj = swirl_program->add_object(rectangle, trans);
        obj->add_texture(scene_texture, "texture_sampler");
        return true;
    }

     bool setup_fisheye(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                     const OpenGLObject &rectangle, shared_prog scene_program){
        FrameBuffer fbo;
        auto scene_texture = fbo.bind_and_attach_to_texture(width, height);
        display();
        std::string vertex_src = load("../src/shaders/fish_eye/vertex.shd");
        std::string fragment_src = load("../src/shaders/fish_eye/fragment.shd");
        shared_prog fisheye_program = Program::make_program(vertex_src, fragment_src);
        if (fisheye_program == NULL)
            return false;
        fisheye_program->use();
        fisheye_program->init_projection_view_matrices(eye, center, up);

        matrix4 trans = matrix4::identity();
        trans.scaled(14, 14, 14);
        trans.translated(-3.2, -3.6, -50);
        auto obj = fisheye_program->add_object(rectangle, trans);
        obj->add_texture(scene_texture, "texture_sampler");
        return true;
    }


}