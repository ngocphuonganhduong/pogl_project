#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector> //need for vbo/*.hh
#include "vbo/triangle.hh"
#include "vbo/teapot.hh"
#include "vbo/rectangle.hh"
#include "init.hh"
#include "stb_image.h"

//#define SAVE_RENDER
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

    if (argc < 2)
    {
        cout << "usage:\n  ./pogl shader_name" << endl;
        cout << "shaders:\n - tp3_test\n - test\n - sepia\n - night_vision\n - glitch\n - depth_map" << endl;
        return 1;
    }

    //path from build folder
    std::string vertex_src = load("../src/shaders/" + string(argv[1]) + "/vertex.shd");
    std::string fragment_src = load("../src/shaders/" + string(argv[1]) + "/fragment.shd");

    shared_prog prog = Program::make_program(vertex_src, fragment_src);

    if (prog->is_ready())
        std::cout << "READY\n";
    else
    {
        std::cout << "NOT READY\n";
        return 1;
    }

    Vector3 eye(0, 0, 17);
    Vector3 center(0, 0, 0);
    Vector3 up(0, 1, 0);
    prog->init_projection_view_matrices(eye, center, up, "projection", "view");


    //init all texture
    stbi_set_flip_vertically_on_load(true);
    shared_text texture1 = Texture::create("../textures/texture.tga");
    prog->add_texture(texture1);
    shared_text brick = Texture::create("../textures/wood.jpg");
    prog->add_texture(brick);
    shared_text red = Texture::create("../textures/redbrick.jpg");
    prog->add_texture(red);

    matrix4 transformation = matrix4::identity();
    transformation.scaled(0.5, 0.5, 0.5);
    transformation.rotated(30, 50,0);
    transformation.translated(-3, -5, -10);


    shared_obj obj1 = prog->add_vbo(teapot_vbd, "position", 3, transformation);
    obj1->add_texture(texture1);

    prog->add_data(normal_smooth_buffer_data, "normal", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D

    //second object
    matrix4 transformation2 = matrix4::identity();
    transformation2.scaled(0.5, 0.5, 0.5);
    transformation2.rotated(-30, 180,20);
    transformation2.translated(2, -5, -20);

    shared_obj obj2 = prog->add_vbo(teapot_vbd, "position", 3, transformation2);
    obj2->add_texture(texture1);

    prog->add_data(normal_smooth_buffer_data, "normal", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D

    //third object
    matrix4 transformation3 = matrix4::identity();
    transformation3.scaled(15, 15, 15);
    transformation3.translated(0, 0, -50);

    shared_obj obj3 = prog->add_vbo(rectangle_vbd, "position", 3, transformation3);
    obj3->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //left

    matrix4 transformation4 = matrix4::identity();
    transformation4.scaled(10, 10, 10);
    transformation4.translated(20, 0, -10);
    transformation4.rotated(0, 85, 0);

    shared_obj obj4 = prog->add_vbo(rectangle_vbd, "position", 3, transformation4);
    obj4->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //right
    matrix4 transformation5 = matrix4::identity();
    transformation5.scaled(10, 10, 10);
    transformation5.translated(-20, 0, -10);
    transformation5.rotated(0, -85, 0);

    shared_obj obj5 = prog->add_vbo(rectangle_vbd, "position", 3, transformation5);
    obj5->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //ground
    matrix4 transformation6 = matrix4::identity();
    transformation6.scaled(10, 10, 10);
    transformation6.translated(0, 20, -10);
    transformation6.rotated(-80, 0, 0);

    shared_obj obj6 = prog->add_vbo(rectangle_vbd, "position", 3, transformation6);
    obj6->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //top
    matrix4 transformation7 = matrix4::identity();
    transformation7.scaled(10, 10, 10);
    transformation7.translated(0, -20, -10);
    transformation7.rotated(80, 0, 0);

    shared_obj obj7 = prog->add_vbo(rectangle_vbd, "position", 3, transformation7);
    obj7->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //fourth object
    matrix4 transformation8 = matrix4::identity();
    transformation8.scaled(0.5, 0.5, 0.5);
    transformation8.rotated(0, 180,0);
    transformation8.translated(-5, -5, -25);

    shared_obj obj8 = prog->add_vbo(teapot_vbd, "position", 3, transformation8);
    obj8->add_texture(texture1);

    prog->add_data(normal_smooth_buffer_data, "normal", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D

    //texture
//    prog->add_texture("../textures/redbrick.tga", "texture_sampler", GL_TEXTURE0);
//    prog->add_texture("../textures/lighting.tga", "lighting_sampler", GL_TEXTURE1);
//    prog->add_texture("../textures/normalmap.tga", "normalmap_sampler", GL_TEXTURE2);


    glutMainLoop();
    return 0;
}
