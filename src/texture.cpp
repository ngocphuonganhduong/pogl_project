//
// Created by rimuru on 14/04/2020.
//

#include <iostream>
#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace pogl
{
    int Texture::max_unit = 0;

    Texture::Texture(const char *filename) {
        unit = max_unit;
        max_unit += 2;
        int img_bpp;
        img_data = stbi_load(filename, &sx, &sy, &img_bpp, 3);
        std::cout << filename << ": " << sx << " ," << sy <<  "\n";

    }

    Texture::~Texture() {
        if (img_data)
            stbi_image_free(img_data);
    }

    shared_text Texture::create(const char *filename) {
        return std::make_shared<Texture>(filename);
    }

    Texture::Texture() {
        unit = max_unit;
        max_unit += 2;
    }
}
