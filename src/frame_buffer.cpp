#include "frame_buffer.hh"
#include "opengl_handling_error.hh"

namespace pogl {

    FrameBuffer::FrameBuffer() {
        glGenFramebuffers(1, &fbo_id);

    }

    GLuint FrameBuffer::bind_and_attach_to_texture(int width, int height) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        TEST_OPENGL_ERROR();

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        TEST_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id);
        TEST_OPENGL_ERROR();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        TEST_OPENGL_ERROR();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);
        TEST_OPENGL_ERROR();

        // The depth buffer
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        TEST_OPENGL_ERROR();
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        TEST_OPENGL_ERROR();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        TEST_OPENGL_ERROR();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        TEST_OPENGL_ERROR();
        return texture_id;
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &fbo_id);
    }

}