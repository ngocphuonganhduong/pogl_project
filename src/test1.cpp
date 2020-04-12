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
        saved = true;
    #endif

    init_glut(argc, argv);
    if (!init_glew())
        return 1;
    init_GL();

    //path from build folder
    std::string vertex_src = load("../src/shaders/test/vertex.shd");
    std::string fragment_src = load("../src/shaders/test/fragment.shd");

    shared_prog prog = Program::make_program(vertex_src, fragment_src);

    shared_prog prog2 = Program::make_program(load("../src/shaders/test/vertex.shd"),
            load("../src/shaders/test/fragment.shd"));

    if (prog->is_ready())
        std::cout << "READY\n";
    else
    {
        std::cout << "NOT READY\n";
        return 1;
    }

    Vector3 eye(2, 2, 17);
    Vector3 center(0, 0, 0);
    Vector3 up(0, 1, 0);
    prog->init_projection_view_matrices(eye, center, up, "projection", "view");
    prog2->init_projection_view_matrices(eye, center, up, "projection", "view");

    matrix4 transformation = matrix4::identity();
    transformation.scaled(1, 0.5, 0.5);
    transformation.translated(-2, 0, 0);


    prog->add_vbo(triangle_vbd, "position", 3, transformation, Vector3(1, 1, 0));
    prog2->add_vbo(triangle_vbd, "position", 3, matrix4::identity(), Vector3(0, 0, 1));

    glutMainLoop();
    return 0;
}
