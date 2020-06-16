#include "obj_loader.hh"

using namespace std;


void Obj::arrange_faces(string str)
{
    string token;
    istringstream stok(str);
    vector<uint64_t> tokens;
    while(getline(stok, token, '/'))
        tokens.push_back(stod(token));
    

    uint64_t index = 3*(tokens[0]-1); 

    vbd.push_back(temp_vbd[index]); //x
    vbd.push_back(temp_vbd[index + 1]); //y
    vbd.push_back(temp_vbd[index + 2]); //z

    index = 2*(tokens[1] - 1); 

    uv.push_back(temp_uv[index]); //x
    uv.push_back(temp_uv[index+1]); //y

    index = 3*(tokens[2] - 1);

    normals.push_back(temp_normals[index]); //x
    normals.push_back(temp_normals[index + 1]); //y
    normals.push_back(temp_normals[index + 2]); //z

}

void Obj::load_file(string filename)
{
    ifstream myfile;
    myfile.open(filename);
    cout << filename << endl;
    if(!myfile)
    {
        cerr << "Unable to open file\n";
        exit(1);
    }
    string line, type, x, y, z;
    while(getline(myfile, line))
    {
        stringstream str(line);
        str >> type;
        if (type == "v")
        {
            str >> x >> y >> z;
            temp_vbd.push_back(stof(x));
            temp_vbd.push_back(stof(y));
            temp_vbd.push_back(stof(z));
        }
        else if (type == "vt")
        {
            str >> x >> y;
            temp_uv.push_back(stof(x));
            temp_uv.push_back(stof(y));
        }
        else if (type == "vn")
        {
            str >> x >> y >> z;
            temp_normals.push_back(stof(x));
            temp_normals.push_back(stof(y));
            temp_normals.push_back(stof(z));
        }
        else if(type == "f")
        {
            str >> x >> y >> z;
            arrange_faces(x); //summit 1
            arrange_faces(y); //summit 2
            arrange_faces(z); //summit 3 of the triangle
        }
    }
}