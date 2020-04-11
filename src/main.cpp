#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector> //need for vbo/*.hh
#include "vbo/triangle.hh"
#include "vbo/teapot.hh"
#include "init.hh"

//#define SAVE_RENDER
using namespace pogl;

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
        saved = false;
    #endif

    init_glut(argc, argv);
    if (!init_glew())
        return 1;
    init_GL();

    //path from build folder
    std::string vertex_src = load("../src/shaders/tp3_test/vertex.shd");
    std::string fragment_src = load("../src/shaders/tp3_test/fragment.shd");

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

    prog->add_vbo(teapot_vbd, "position", 3);
    prog->add_data(normal_flat_buffer_data, "normalFlat", 3);
    prog->add_data(normal_smooth_buffer_data, "normalSmooth", 3);
    prog->add_data(color_buffer_data, "color", 3);
    prog->add_data(uv_buffer_data, "uv", 2); //2D

    prog->add_texture("../textures/texture.tga", "texture_sampler", GL_TEXTURE0);
    prog->add_texture("../textures/lighting.tga", "lighting_sampler", GL_TEXTURE1);
    prog->add_texture("../textures/normalmap.tga", "normalmap_sampler", GL_TEXTURE2);

    glutMainLoop();
    return 0;
}
