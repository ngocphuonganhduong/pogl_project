#include <iostream>
#include "texture.hh"
#include "opengl_handling_error.hh"
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

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        TEST_OPENGL_ERROR();
        glActiveTexture(GL_TEXTURE0 + unit);
        TEST_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sx, sy, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        TEST_OPENGL_ERROR();
    }

    Texture::~Texture() {
        if (img_data)
            stbi_image_free(img_data);
    }

    shared_text Texture::create(const char *filename) {
        return std::make_shared<Texture>(filename);
    }

    Texture::Texture(GLuint texture_id_, GLsizei width, GLsizei height) {
        unit = max_unit;
        max_unit += 2;
        texture_id = texture_id_;
        glActiveTexture(GL_TEXTURE0 + unit);
        TEST_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
    }
}
