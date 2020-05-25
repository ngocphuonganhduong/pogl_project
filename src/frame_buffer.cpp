//
// Created by rimuru on 24/05/2020.
//

#include "frame_buffer.hh"
#include "program.hh"

namespace pogl {

    FrameBuffer::FrameBuffer() {
        glGenFramebuffers(1, &fbo_id);

    }

    GLuint FrameBuffer::bind_and_attach_to_texture(int width, int height) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);

        // The depth buffer
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        return texture_id;
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &fbo_id);
    }

}