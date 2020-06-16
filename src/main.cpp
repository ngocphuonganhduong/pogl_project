#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "vbo/teapot.hh"
#include "vbo/rectangle.hh"
#include "init.hh"
#include "stb_image.h"
#include "frame_buffer.hh"
#include "obj_loader.hh"
#include "opengl_handling_error.hh"

using namespace pogl;
using namespace std;

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


int main(int argc, char *argv[]) {
    width = 1024;
    height = 1024;
#if defined(SAVE_RENDER)
    saved = true;
#endif

    init_glut(argc, argv);
    if (!init_glew())
        return 1;
    init_GL();

    if (argc < 2) {
        cout << "usage:\n  ./pogl shader_name" << endl;
        cout
                << "shaders:\n - tp3_test\n - sepia\n - night_vision\n - glitch\n - depth_map\n - anaglyph - depth_of_field"
                << endl;
        return 1;
    }
    stbi_set_flip_vertically_on_load(true);

    std::string shader_name(argv[1]);
    std::string main_scene(argv[1]);
    //path from build folder
    if (shader_name == "anaglyph") {
        main_scene = "simple";
    } else if (shader_name == "depth_of_field") {
        main_scene = "depth_map";
    }
    /** init TEXTURE **/
    shared_text texture1 = Texture::create("../textures/texture.tga");
    shared_text brick = Texture::create("../textures/wood.jpg");
    shared_text red = Texture::create("../textures/redbrick.jpg");

    /** init OpenGLObject - vao, vbo **/
    OpenGLObject teapot(teapot_vbd, normal_smooth_buffer_data, uv_buffer_data);
    OpenGLObject rectangle(rectangle_vbd, rectangle_normal, rectangle_uv);


    /** init a program and use it **/
    std::string vertex_src = load("../src/shaders/" + main_scene + "/vertex.shd");
    std::string fragment_src = load("../src/shaders/" + main_scene + "/fragment.shd");

    shared_prog prog = Program::make_program(vertex_src, fragment_src);
    prog->use();
    if (prog->is_ready())
        std::cout << "READY\n";
    else {
        std::cout << "NOT READY\n";
        return 1;
    }
    Vector3 eye(0, 0, 17);
    Vector3 center(0, 0, 0);
    Vector3 up(0, 1, 0);
    prog->init_projection_view_matrices(eye, center, up, "projection", "view");

    /** init PROGRAM OBJECTS - add opengl objects to program**/
    //teapot 1
    matrix4 transformation = matrix4::identity();
    transformation.scaled(0.5, 0.5, 0.5);
    transformation.rotated(30, 50, 0);
    transformation.translated(-3, -5, -10);
    auto obj1 = prog->add_object(teapot, transformation);
    obj1->add_texture(texture1, "texture_sampler");

    //teapot 2
    matrix4 transformation2 = matrix4::identity();
    transformation2.scaled(0.5, 0.5, 0.5);
    transformation2.rotated(-30, 180, 20);
    transformation2.translated(2, -5, -20);
    auto obj2 = prog->add_object(teapot, transformation2);
    obj2->add_texture(texture1, "texture_sampler");

    //front wall
    matrix4 transformation3 = matrix4::identity();
    transformation3.scaled(15, 15, 15);
    transformation3.translated(0, 0, -50);
    auto obj3 = prog->add_object(rectangle, transformation3);
    obj3->add_texture(brick, "texture_sampler");

    //left wall
    matrix4 transformation4 = matrix4::identity();
    transformation4.scaled(10, 10, 10);
    transformation4.translated(20, 0, -10);
    transformation4.rotated(0, 85, 0);
    auto obj4 = prog->add_object(rectangle, transformation4);
    obj4->add_texture(brick, "texture_sampler");

    //right wall
    matrix4 transformation5 = matrix4::identity();
    transformation5.scaled(10, 10, 10);
    transformation5.translated(-20, 0, -10);
    transformation5.rotated(0, -85, 0);
    auto obj5 = prog->add_object(rectangle, transformation5);
    obj5->add_texture(brick, "texture_sampler");

    //ground
    matrix4 transformation6 = matrix4::identity();
    transformation6.scaled(10, 10, 10);
    transformation6.translated(0, 20, -10);
    transformation6.rotated(-80, 0, 0);
    auto obj6 = prog->add_object(rectangle, transformation6);
    obj6->add_texture(brick, "texture_sampler");

    //roof
    matrix4 transformation7 = matrix4::identity();
    transformation7.scaled(10, 10, 10);
    transformation7.translated(0, -20, -10);
    transformation7.rotated(80, 0, 0);
    auto obj7 = prog->add_object(rectangle, transformation7);
    obj7->add_texture(brick, "texture_sampler");

    //teapot 3
    matrix4 transformation8 = matrix4::identity();
    transformation8.scaled(0.5, 0.5, 0.5);
    transformation8.rotated(0, 180, 0);
    transformation8.translated(-5, -5, -25);
    auto obj8 = prog->add_object(teapot, transformation8);
    obj8->add_texture(texture1, "texture_sampler");

    if (shader_name == "anaglyph") {
        //CREATE a framebuffer to render current scene to a texture
        FrameBuffer fbo;
        GLuint texture_left_id = fbo.bind_and_attach_to_texture(width, height);
        display();
        FrameBuffer fbo2;
        GLuint texture_right_id = fbo.bind_and_attach_to_texture(width, height);

        matrix4 projection = matrix4::identity();
        float ratio = width / height;
        frustum(projection, -ratio + 0.04, ratio + 0.04, -1, 1, 5, 50000);
        GLuint pro_mat_id = glGetUniformLocation(prog->program_id(), "projection");
        TEST_OPENGL_ERROR();
        glUniformMatrix4fv(pro_mat_id, 1, GL_FALSE, &projection.mat[0][0]);
        TEST_OPENGL_ERROR();

        display();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        vertex_src = load("../src/shaders/anaglyph/vertex.shd");
        fragment_src = load("../src/shaders/anaglyph/fragment.shd");
        shared_prog prog2 = Program::make_program(vertex_src, fragment_src);
        if (!prog2->is_ready())
            return 1;
        prog2->use();
        prog2->init_projection_view_matrices(eye, center, up, "projection", "view");

        matrix4 trans = matrix4::identity();
        trans.scaled(12, 12, 12);
        trans.translated(0, 0, -50);
        auto obj = prog2->add_object(rectangle, trans);

        shared_text texture_left = std::make_shared<Texture>();
        glActiveTexture(GL_TEXTURE0 + texture_left->unit);
        glBindTexture(GL_TEXTURE_2D, texture_left_id);
        obj->add_texture(texture_left, "texture_left");
        shared_text texture_right = std::make_shared<Texture>();
        glActiveTexture(GL_TEXTURE0 + texture_right->unit);
        glBindTexture(GL_TEXTURE_2D, texture_right_id);
        obj->add_texture(texture_right, "texture_right");
    }
    else if(shader_name == "depth_of_field")
    {
        FrameBuffer fbo_depth_map;
        GLuint texture_depth_map_id = fbo_depth_map.bind_and_attach_to_texture(width, height);
        display();
        std::string vertex_src2 = load("../src/shaders/" + main_scene + "/vertex.shd");
        std::string fragment_src2 = load("../src/shaders/" + main_scene + "/fragment.shd");

    }

    glutMainLoop();
    return 0;
}