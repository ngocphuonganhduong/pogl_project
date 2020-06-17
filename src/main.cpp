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
    if (shader_name == "anaglyph" || shader_name == "depth_of_field") {
        main_scene = "simple";
    }

    /** init TEXTURE **/
    shared_text texture1 = Texture::create("../textures/texture.tga");
    shared_text brick = Texture::create("../textures/wood.jpg");
    shared_text white = Texture::create("../textures/white.png");

    /** init OpenGLObject - vao, vbo **/
    OpenGLObject teapot(teapot_vbd, normal_smooth_buffer_data, uv_buffer_data);
    OpenGLObject rectangle(rectangle_vbd, rectangle_normal, rectangle_uv);

    Obj bunny_obj = Obj();
    bunny_obj.load_file("../src/vbo/bunny.obj");
    OpenGLObject bunny(bunny_obj.get_vbd(), bunny_obj.get_normals(), bunny_obj.get_uv());

    Obj cube_obj = Obj();
    cube_obj.load_file("../src/vbo/cube.obj");
    OpenGLObject cube(cube_obj.get_vbd(), cube_obj.get_normals(), cube_obj.get_uv());

    Obj table_obj = Obj();
    table_obj.load_file("../src/vbo/table.obj");
    OpenGLObject table(table_obj.get_vbd(), table_obj.get_normals(), table_obj.get_uv());

    Obj chair_obj = Obj();
    chair_obj.load_file("../src/vbo/chair.obj");
    OpenGLObject chair(chair_obj.get_vbd(), chair_obj.get_normals(), chair_obj.get_uv());

    Obj cup_obj = Obj();
    cup_obj.load_file("../src/vbo/cup.obj");
    OpenGLObject cup(cup_obj.get_vbd(), cup_obj.get_normals(), cup_obj.get_uv());

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
    prog->init_projection_view_matrices(eye, center, up);

    /** init PROGRAM OBJECTS - add opengl objects to program**/

    //front wall
    matrix4 transformation1 = matrix4::identity();
    transformation1.scaled(15, 15, 15);
    transformation1.translated(0, 0, -50);
    auto obj1 = prog->add_object(rectangle, transformation1);
    obj1->add_texture(brick, "texture_sampler");

    //left wall
    matrix4 transformation2 = matrix4::identity();
    transformation2.scaled(10, 10, 10);
    transformation2.translated(20, 0, -10);
    transformation2.rotated(0, 85, 0);
    auto obj2 = prog->add_object(rectangle, transformation2);
    obj2->add_texture(brick, "texture_sampler");

    //right wall
    matrix4 transformation3 = matrix4::identity();
    transformation3.scaled(10, 10, 10);
    transformation3.translated(-20, 0, -10);
    transformation3.rotated(0, -85, 0);
    auto obj3 = prog->add_object(rectangle, transformation3);
    obj3->add_texture(brick, "texture_sampler");

    //ground
    matrix4 transformation4 = matrix4::identity();
    transformation4.scaled(10, 10, 10);
    transformation4.translated(0, 20, -10);
    transformation4.rotated(-80, 0, 0);
    auto obj4 = prog->add_object(rectangle, transformation4);
    obj4->add_texture(brick, "texture_sampler");

    //top
    matrix4 transformation5 = matrix4::identity();
    transformation5.scaled(10, 10, 10);
    transformation5.translated(0, -20, -10);
    transformation5.rotated(80, 0, 0);
    auto obj5 = prog->add_object(rectangle, transformation5);
    obj5->add_texture(brick, "texture_sampler");

    //bunny
    matrix4 transformation7 = matrix4::identity();
    transformation7.scaled(15, 15, 15);
    transformation7.rotated(0, 45, 0);
    transformation7.translated(-4, -5, -20);
    auto obj7 = prog->add_object(bunny, transformation7);
    obj7->add_texture(white, "texture_sampler");


    //box
    matrix4 transformation8 = matrix4::identity();
    transformation8.scaled(0.5, 0.5, 0.5);
    transformation8.rotated(0, 0, 0);
    transformation8.translated(-5, -5, -20);
    auto obj8 = prog->add_object(cube, transformation8);
    obj8->add_texture(white, "texture_sampler");

    //teapot
    matrix4 teapot_transformation = matrix4::identity();
    teapot_transformation.scaled(0.25, 0.25, 0.25);
    teapot_transformation.rotated(0, 180, 0);
    teapot_transformation.translated(4.5, -4.125, -24);;
    auto obj6 = prog->add_object(teapot, teapot_transformation);
    obj6->add_texture(texture1, "texture_sampler");

    //cup
    matrix4 cup_transformation = matrix4::identity();
    cup_transformation.scaled(0.9, 0.9, 0.9);
    cup_transformation.rotated(0, 180, 0);
    cup_transformation.translated(3.5, -4.125, -24);;
    auto obj11 = prog->add_object(cup, cup_transformation);
    obj11->add_texture(white, "texture_sampler");

    //table
    matrix4 table_transformation = matrix4::identity();
    table_transformation.scaled(0.04, 0.04, 0.04);
    table_transformation.rotated(0, 90, 90);
    table_transformation.translated(4, -6, -25);
    auto obj9 = prog->add_object(table, table_transformation);
    obj9->add_texture(white, "texture_sampler");

    matrix4 transformation_chair = matrix4::identity();
    transformation_chair.scaled(1.2, 1.2, 1.2);
    transformation_chair.rotated(0, -20, 0);
    transformation_chair.translated(10, -6, -20);
    auto obj10 = prog->add_object(chair, transformation_chair);


    if (shader_name == "anaglyph") {
        //CREATE a framebuffer to render current scene to a texture
        FrameBuffer fbo;
        auto left_texture = fbo.bind_and_attach_to_texture(width, height);
        display();

        FrameBuffer fbo2;
        auto right_texture = fbo.bind_and_attach_to_texture(width, height);
        matrix4 projection = matrix4::identity();
        float ratio = width / height;
        frustum(projection, -ratio + 0.04, ratio + 0.04, -1, 1, 5, 50000);
        GLuint pro_mat_id = glGetUniformLocation(prog->program_id(), "projection");
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
        prog2->init_projection_view_matrices(eye, center, up);

        matrix4 trans = matrix4::identity();
        trans.scaled(12, 12, 12);
        trans.translated(0, 0, -50);
        auto obj = prog2->add_object(rectangle, trans);
        obj->add_texture(left_texture, "texture_left");
        obj->add_texture(right_texture, "texture_right");
    } else if (shader_name == "depth_of_field") {
        FrameBuffer fbo1;
        auto scene_texture = fbo1.bind_and_attach_to_texture(width, height);
        display();

//        //depth map
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        std::string vertex_src2 = load("../src/shaders/depth_of_field/vertex.shd");
        std::string fragment_src2 = load("../src/shaders/depth_of_field/ndc_depth_fragment.shd");
        shared_prog depth_map_program = Program::make_program(vertex_src2, fragment_src2);
        if (!depth_map_program->is_ready())
            return 1;
        depth_map_program->use();
        depth_map_program->copy(prog);
        FrameBuffer fbo2;
        auto depth_texture = fbo2.bind_and_attach_to_texture(width, height);
        display();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        std::string vertex_src3 = load("../src/shaders/depth_of_field/vertex.shd");
        std::string fragment_src3 = load("../src/shaders/depth_of_field/fragment.shd");
        shared_prog prog3 = Program::make_program(vertex_src3, fragment_src3);
        if (!prog3->is_ready())
            return 1;
        prog3->use();
        prog3->init_projection_view_matrices(eye, center, up);
        matrix4 trans = matrix4::identity();
        trans.scaled(12, 12, 12);
        trans.translated(0, 0, -50);
        auto obj = prog3->add_object(rectangle, trans);
        obj->add_texture(scene_texture, "scene_texture");
        obj->add_texture(depth_texture, "depth_texture");
    }

    glutMainLoop();
    return 0;
}