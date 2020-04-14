//
// Created by rimuru on 14/04/2020.
//

#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <cstdint>
#include <memory>

namespace pogl {
    class Texture {
    public:
        ~Texture();

        static std::shared_ptr<Texture> create(const char *filename);

        Texture(const char *filename);

        int sx;
        int sy;
        uint8_t *img_data;
        int unit;//opengl texture unit
    private:
        static int max_unit;
    };

    using shared_text = std::shared_ptr<Texture>;
}


#endif //TEXTURE_HH
