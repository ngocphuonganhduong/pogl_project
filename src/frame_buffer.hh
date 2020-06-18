#ifndef FRAME_BUFFER_HH
#define FRAME_BUFFER_HH

#include <GL/glew.h>
#include "texture.hh"

namespace pogl {
    class FrameBuffer {
    public:
        FrameBuffer();

        ~FrameBuffer();

        shared_text bind_and_attach_to_texture(int width, int height);

        GLuint fbo_id;
    };
}

#endif //FRAME_BUFFER_HH
