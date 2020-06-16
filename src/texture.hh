#ifndef TEXTURE_HH
#define TEXTURE_HH
#include <cstdint>
#include <memory>
#include <GL/glew.h>

namespace pogl {
    class Texture {
    public:
        ~Texture();

        static std::shared_ptr<Texture> create(const char *filename);

        Texture(const char *filename);
        Texture();

        int sx;
        int sy;
        uint8_t *img_data = NULL;
        int unit;//opengl texture unit
    private:
        GLuint texture_id;
        static int max_unit;
    };

    using shared_text = std::shared_ptr<Texture>;
}


#endif //TEXTURE_HH
