//
// Created by rimuru on 11/04/2020.
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "init.hh"
#include "utils/image_io.hh"
#include "utils/misc.hh"

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

    void timer_func(int v)
    {
        glutPostRedisplay();
    }
    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TEST_OPENGL_ERROR();
        for (auto p: programs)
        {
            int location = glGetUniformLocation(p->program_id(), "current_time");
            glUniform1i(location,  glutGet(GLUT_ELAPSED_TIME));
            TEST_OPENGL_ERROR();
            location = glGetUniformLocation(p->program_id(), "rnd");
            glUniform1f(location,random1());
            TEST_OPENGL_ERROR();
            location = glGetUniformLocation(p->program_id(), "rnd2");
            glUniform1f(location, drand48());
            TEST_OPENGL_ERROR();
        }

        for (auto o: objects) {
            o->draw();
        }
        glBindVertexArray(0);
        if (saved) {
            tifo::rgb24_image *texture = new tifo::rgb24_image(800, 590);
            glReadPixels(150, 250, 800, 590, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
            TEST_OPENGL_ERROR();
            //glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
            tifo::save_image(*texture, "render.tga");
            std::cout << "Save " << std::endl;
            delete texture;
            //saved = true;
        }
        glutSwapBuffers();
        glutTimerFunc(fpms, timer_func, 0);
    }


    void init_glut(int &argc, char *argv[]) {
        //glewExperimental = GL_TRUE;
        glutInit(&argc, argv);
        glutInitContextVersion(4, 2);
        glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(width, height);
        glutInitWindowPosition(10, 10);
        glutCreateWindow("pogl");
        glutDisplayFunc(display);
        glutReshapeFunc(window_resize);
    }


}