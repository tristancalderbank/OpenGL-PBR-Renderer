#include "ibl/brdfconvolutionframebuffer.h"

#include "glad/glad.h"

BrdfConvolutionFramebuffer::BrdfConvolutionFramebuffer(unsigned int width, unsigned int height) : width(width), height(height) {
    // framebuffer
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // depth buffer
    glGenRenderbuffers(1, &depthRenderbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    // attach the depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferId);

    // color texture
    glGenTextures(1, &colorTextureId);
    glBindTexture(GL_TEXTURE_2D, colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BrdfConvolutionFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
}

unsigned int BrdfConvolutionFramebuffer::getWidth()
{
    return width;
}

unsigned int BrdfConvolutionFramebuffer::getHeight()
{
    return height;
}

unsigned int BrdfConvolutionFramebuffer::getColorTextureId() {
    return colorTextureId;
}
