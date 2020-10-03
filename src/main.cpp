#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "vbo/teapot.hh"
#include "vbo/rectangle.hh"
#include "init.hh"
#include "stb_image.h"
#include "frame_buffer.hh"
#include "obj_loader.hh"
#include "setup_shader.hh"

using namespace pogl;
using namespace std;

void print_help() {
    cout << "usage:\n  ./pogl shader_name" << endl;
    cout << "shaders:\n - simple\n - sepia\n - night_vision\n - glitch\n";
    cout << " - depth_map\n - anaglyph\n - depth_of_field\n - stroboscope\n";
    cout << " - glitter\n - swirl\n - fish_eye" << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 0;
    }
    std::string shader_name(argv[1]);
    std::string main_scene(argv[1]);

    //path from build folder
    if (shader_name == "anaglyph" || shader_name == "depth_of_field" ||
        shader_name == "glitch" || shader_name == "swirl" || shader_name == "fish_eye") {
        main_scene = "simple";
    }

    /** load shader sources **/
    std::string vertex_src = load("../src/shaders/" + main_scene + "/vertex.shd");
    std::string fragment_src = load("../src/shaders/" + main_scene + "/fragment.shd");
    if (vertex_src.empty() || fragment_src.empty()) {
        print_help();
        return 1;
    }

    width = 1024;
    height = 900;
#if defined(SAVE_RENDER)
    saved = true;
#endif

    init_glut(argc, argv);
    if (!init_glew())
        return 1;
    init_GL();

    stbi_set_flip_vertically_on_load(true);

    /** init TEXTURE **/
    shared_text texture1 = Texture::create("../textures/texture.tga");
    shared_text wall_texture = Texture::create("../textures/cream_brick.jpg");
    shared_text brick = Texture::create("../textures/wood.jpg");
    shared_text plant_texture = Texture::create("../textures/plant.tga");
    shared_text white = Texture::create("../textures/white.png");
    shared_text sofa_texture = Texture::create("../textures/blue_fabric.jpg");
    shared_text door_texture = Texture::create("../textures/door.jpg");
    shared_text rug_texture = Texture::create("../textures/pink_rug.jpeg");
    shared_text floor_texture = Texture::create("../textures/floor.jpg");

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

    Obj plant_obj = Obj();
    plant_obj.load_file("../src/vbo/plant.obj");
    OpenGLObject plant(plant_obj.get_vbd(), plant_obj.get_normals(), plant_obj.get_uv());

    Obj sofa_obj = Obj();
    sofa_obj.load_file("../src/vbo/sofa.obj");
    OpenGLObject sofa(sofa_obj.get_vbd(), sofa_obj.get_normals(), sofa_obj.get_uv());


    Obj door_obj = Obj();
    door_obj.load_file("../src/vbo/door.obj");
    OpenGLObject door(door_obj.get_vbd(), door_obj.get_normals(), door_obj.get_uv());

    Obj armchair_obj = Obj();
    armchair_obj.load_file("../src/vbo/armchair.obj");
    OpenGLObject armchair(armchair_obj.get_vbd(), armchair_obj.get_normals(), armchair_obj.get_uv());

    /** init a program and use it **/
    shared_prog prog = Program::make_program(vertex_src, fragment_src);
    if (prog == NULL)
        return 1;
    prog->use();

    Vector3 eye(0.5, 0, 20);
    Vector3 center(-0.5, -1, 0);
    Vector3 up(0, 1, 0);
    prog->init_projection_view_matrices(eye, center, up);

    /** init PROGRAM OBJECTS - add opengl objects to program**/

    //front wall
    matrix4 transformation1 = matrix4::identity();
    transformation1.scaled(10, 8, 8);
    transformation1.translated(-1, 0, -40);
    auto obj1 = prog->add_object(rectangle, transformation1);
    obj1->add_texture(wall_texture, "texture_sampler");

    //left wall
    matrix4 transformation2 = matrix4::identity();
    transformation2.scaled(8, 8, 8);
    transformation2.translated(24, 0, -12);
    transformation2.rotated(0, 85, 0);
    auto obj2 = prog->add_object(rectangle, transformation2);
    obj2->add_texture(wall_texture, "texture_sampler");

    //right wall
    matrix4 transformation3 = matrix4::identity();
    transformation3.scaled(10, 10, 10);
    transformation3.translated(-20, 0, -10);
    transformation3.rotated(0, -85, 0);
    auto obj3 = prog->add_object(rectangle, transformation3);
    obj3->add_texture(wall_texture, "texture_sampler");

    //ground
    matrix4 transformation4 = matrix4::identity();
    transformation4.scaled(10, 11, 10);
    transformation4.translated(0, 21, -10);
    transformation4.rotated(-80, 0, 0);
    auto obj4 = prog->add_object(rectangle, transformation4);
    obj4->add_texture(floor_texture, "texture_sampler");

    //top
    matrix4 transformation5 = matrix4::identity();
    transformation5.scaled(10, 10, 10);
    transformation5.translated(0, -20, -11);
    transformation5.rotated(80, 0, 0);
    auto obj5 = prog->add_object(rectangle, transformation5);
    obj5->add_texture(floor_texture, "texture_sampler");

    //bunny
    matrix4 transformation7 = matrix4::identity();
    transformation7.scaled(15, 15, 15);
    transformation7.rotated(0, 45, 0);
    transformation7.translated(-6, -5, -25);
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
    teapot_transformation.translated(3, -4.125, -4);;
    auto obj6 = prog->add_object(teapot, teapot_transformation);
    obj6->add_texture(texture1, "texture_sampler");

    //cup
    matrix4 cup_transformation = matrix4::identity();
    cup_transformation.scaled(0.9, 0.9, 0.9);
    cup_transformation.rotated(0, 180, 0);
    cup_transformation.translated(2, -4.125, -5);;
    auto obj11 = prog->add_object(cup, cup_transformation);
    obj11->add_texture(white, "texture_sampler");

    //table
    matrix4 table_transformation = matrix4::identity();
    table_transformation.scaled(0.04, 0.04, 0.04);
    table_transformation.rotated(0, 90, 90);
    table_transformation.translated(3, -6, -5);
    auto obj9 = prog->add_object(table, table_transformation);
    obj9->add_texture(white, "texture_sampler");

    matrix4 transformation_chair = matrix4::identity();
    transformation_chair.scaled(1.2, 1.2, 1.2);
    transformation_chair.rotated(0, -20, 0);
    transformation_chair.translated(9, -6, -0);
    auto obj10 = prog->add_object(chair, transformation_chair);

    matrix4 transformation_plant = matrix4::identity();
    transformation_plant.scaled(1, 1, 1);
    transformation_plant.translated(4.5, -5, -30);
    auto obj12 = prog->add_object(plant, transformation_plant);
    obj12->add_texture(plant_texture, "texture_sampler");

    matrix4 transformation_sofa = matrix4::identity();
    transformation_sofa.scaled(0.4, 0.4, 0.4);
    transformation_sofa.rotated(-90, 0, 0);
    transformation_sofa.translated(-1, -4, -30);
    auto obj13 = prog->add_object(sofa, transformation_sofa);
    obj13->add_texture(sofa_texture, "texture_sampler");

    matrix4 transformation_door = matrix4::identity();
    transformation_door.scaled(0.03, 0.03, 0.03);
    transformation_door.rotated(-90, 85, 0);
    transformation_door.translated(-9.75, -6, -25);
    auto obj14 = prog->add_object(door, transformation_door);
    obj14->add_texture(door_texture, "texture_sampler");


    matrix4 transformation_armchair = matrix4::identity();
    transformation_armchair.scaled(0.3, 0.3, 0.3);
    transformation_armchair.rotated(0, -40, 0);
    transformation_armchair.translated(6, -5, -24);
    auto obj15 = prog->add_object(armchair, transformation_armchair);
    obj15->add_texture(rug_texture, "texture_sampler");

    if (shader_name == "glitch") {
        if (!setup_glitch(eye, center, up, rectangle))
            return 1;
    } else if (shader_name == "anaglyph") {
        if (!setup_anaglyph(eye, center, up, rectangle, prog))
            return 1;
    } else if (shader_name == "depth_of_field") {
        Obj plane_obj = Obj();
        plane_obj.load_file("../src/vbo/paper_airplane.obj");
        OpenGLObject plane(plane_obj.get_vbd(), plane_obj.get_normals(), plane_obj.get_uv());
        matrix4 transformation_paper = matrix4::identity();
        transformation_paper.scaled(1, 1, 1);
        transformation_paper.rotated(0, 0, 30);
        transformation_paper.rotated(-45, 90, 0);
        transformation_paper.translated(0.25, -0.5, 14);
        auto obj16 = prog->add_object(plane, transformation_paper);
        obj16->add_texture(white, "texture_sampler");
        if (!setup_depth_of_field(eye, center, up, rectangle, prog))
            return 1;
    } else if (shader_name == "swirl") {
        if (!setup_swirl(eye, center, up, rectangle, prog))
            return 1;
    } else if (shader_name == "fish_eye") {
        if (!setup_fisheye(eye, center, up, rectangle, prog))
            return 1;
    }
    glutMainLoop();
    return 0;
}