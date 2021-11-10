#include "framebuffer.h"

#include <glad/glad.h>

Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {}

static void initColorTexture(unsigned int colorTextureId, int attachmentSlot, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // attach the color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentSlot, GL_TEXTURE_2D, colorTextureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void
Framebuffer::init() {
    // create the framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // create a color texture
    glGenTextures(1, &colorTexture);
    glGenTextures(1, &bloomColorTexture);

    initColorTexture(colorTexture, GL_COLOR_ATTACHMENT0, width, height);
    initColorTexture(bloomColorTexture, GL_COLOR_ATTACHMENT1, width, height);

    // create depth/stencil buffer
    // we use renderbuffer which is similar to textures except you can't sample it
    // that is okay because we never actually need to read the values in depth/stencil buffer
    // its all done by hardware depth/stencil tests
    glGenRenderbuffers(1, &depthStencilRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach the renderbuffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderbuffer);

    unsigned int colorAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, colorAttachments);

    // make sure the framebuffer was created successfully
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Error initializing framebuffer: framebuffer not complete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
Framebuffer::resize(int width, int height) {
    // resize color textures
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, bloomColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // resize renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned int
Framebuffer::getFramebufferId() {
    return framebuffer;
}

unsigned int
Framebuffer::getColorTextureId() {
    return colorTexture;
}

unsigned int
Framebuffer::getBloomColorTextureId() {
    return bloomColorTexture;
}
