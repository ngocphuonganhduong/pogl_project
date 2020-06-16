#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

class Obj
{
    public:
        Obj() = default;
        void load_file(string filename);
        void arrange_faces(string str);
        vector<GLfloat> get_vbd(){return vbd;};
        vector<GLfloat> get_temp_vbd(){return temp_vbd;};
        vector<GLfloat> get_uv(){return uv;};
        vector<GLfloat> get_normals(){return normals;};

    private:
        vector<GLfloat> temp_vbd, temp_uv, temp_normals;
        vector<GLfloat> vbd, uv, normals;
};