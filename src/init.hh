//
// Created by rimuru on 11/04/2020.
//
#include "program.hh"

#ifndef INIT_HH
#define INIT_HH

namespace pogl {
    void init_GL();

    bool init_glew();

    void window_resize(int width, int height);

    void display();

    void init_glut(int &argc, char *argv[]);
}

#endif //INIT_HH
