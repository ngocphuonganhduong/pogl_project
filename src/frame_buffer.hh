//
// Created by rimuru on 24/05/2020.
//

#ifndef FRAME_BUFFER_HH
#define FRAME_BUFFER_HH

#include <GL/glew.h>

namespace pogl {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        GLuint bind_and_attach_to_texture(int width, int height);
        GLuint fbo_id;
    };
}



#endif //FRAME_BUFFER_HH
