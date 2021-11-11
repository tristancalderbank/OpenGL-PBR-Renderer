#include "bloomframebuffer.h"

#include <glad/glad.h>

BloomFramebuffer::BloomFramebuffer(int width, int height) : mWidth(width), mHeight(height) {}

void
BloomFramebuffer::init() {
    // create the framebuffer
    glGenFramebuffers(1, &mFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

    // create a color texture
    glGenTextures(1, &mColorTextureId);
    glBindTexture(GL_TEXTURE_2D, mColorTextureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // attach the color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTextureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
BloomFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);
}

void
BloomFramebuffer::resize(int width, int height) {
    // resize color textures
    glBindTexture(GL_TEXTURE_2D, mColorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int
BloomFramebuffer::getFramebufferId() {
    return mFramebufferId;
}

unsigned int
BloomFramebuffer::getColorTextureId() {
    return mColorTextureId;
}