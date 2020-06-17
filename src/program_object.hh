#ifndef PROGRAM_OBJECT_HH
#define PROGRAM_OBJECT_HH

#include "opengl_object.hh"
#include "utils/matrix4.hh"

#include <unordered_map>

namespace pogl {
    class ProgramObject {
    public:
        ProgramObject(GLuint program_id_, const OpenGLObject& obj_) : program_id(program_id_),obj(obj_) {};

        void draw(GLuint program_id);

        void add_texture(shared_text texture, std::string name);

        void set_transformation(const matrix4 &transformation);

    private:
        GLuint program_id;
        OpenGLObject obj;
        matrix4 transformation;
        Vector3 uniform_color;
        std::unordered_map<std::string, shared_text> textures;
    };

    using shared_prog_obj = std::shared_ptr<ProgramObject>;

}


#endif //PROGRAM_OBJECT_HH
