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
        cout << "shaders:\n - tp3_test\n - test\n - sepia\n - night_vision" << endl;
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
    shared_text brick = Texture::create("../textures/redbrick.jpg");
    prog->add_texture(brick);


    matrix4 transformation = matrix4::identity();
    transformation.scaled(0.5, 0.5, 0.5);
    transformation.rotated(30, 30,0);
    transformation.translated(-2, 0, -10);


    shared_obj obj1 = prog->add_vbo(teapot_vbd, "position", 3, transformation);
    obj1->add_texture(texture1);

    prog->add_data(normal_smooth_buffer_data, "normal", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D



    //second object
    matrix4 transformation2 = matrix4::identity();
    transformation2.scaled(0.5, 0.5, 0.5);
    transformation2.rotated(-30, 180,20);
    transformation2.translated(2, 0, -10);

    shared_obj obj2 = prog->add_vbo(teapot_vbd, "position", 3, transformation2);
    obj2->add_texture(texture1);

    prog->add_data(normal_smooth_buffer_data, "normal", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D

    //third object

    matrix4 transformation3 = matrix4::identity();
    transformation3.scaled(5, 5, 5);
    transformation3.translated(0, 0, -15);


    shared_obj obj3 = prog->add_vbo(rectangle_vbd, "position", 3, transformation3);
    obj3->add_texture(brick);
    prog->add_data(rectangle_normal, "normal", 3);
    prog->add_data(rectangle_uv, "uv", 2); //2D

    //texture
//    prog->add_texture("../textures/redbrick.tga", "texture_sampler", GL_TEXTURE0);
//    prog->add_texture("../textures/lighting.tga", "lighting_sampler", GL_TEXTURE1);
//    prog->add_texture("../textures/normalmap.tga", "normalmap_sampler", GL_TEXTURE2);




    glutMainLoop();
    return 0;
}
