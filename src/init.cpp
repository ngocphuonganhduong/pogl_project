//
// Created by rimuru on 11/04/2020.
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "init.hh"
#include "image_io.hh"

namespace pogl {

    void init_GL() {
        glEnable(GL_DEPTH_TEST);
        TEST_OPENGL_ERROR();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        TEST_OPENGL_ERROR();
        glEnable(GL_CULL_FACE);
        TEST_OPENGL_ERROR();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        TEST_OPENGL_ERROR();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }

    bool init_glew() {
        if (glewInit()) {
            std::cerr << " Error while initializing glew";
            return false;
        }
        return true;
    }

    void window_resize(int width, int height) {
        glViewport(0, 0, width, height);
        TEST_OPENGL_ERROR();
    }


    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TEST_OPENGL_ERROR();
        for (auto vbo: vbo_list) {
            glBindVertexArray(vbo.first);
            glDrawArrays(GL_TRIANGLES, 0, vbo.second.size());
            TEST_OPENGL_ERROR();
            glBindVertexArray(0);
            TEST_OPENGL_ERROR();

        }

        if (saved) {
            tifo::rgb24_image *texture = new tifo::rgb24_image(800, 590);
            glReadPixels(150, 350, 800, 590, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
            TEST_OPENGL_ERROR();
            //glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
            tifo::save_image(*texture, "render.tga");
            std::cout << "Save " << std::endl;
            delete texture;
            //saved = true;
        }
        glutSwapBuffers();

    }

    void init_glut(int &argc, char *argv[]) {
        //glewExperimental = GL_TRUE;
        glutInit(&argc, argv);
        glutInitContextVersion(4, 5);
        glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(width, height);
        glutInitWindowPosition(10, 10);
        glutCreateWindow("pogl");
        glutDisplayFunc(display);
        glutReshapeFunc(window_resize);
    }


}