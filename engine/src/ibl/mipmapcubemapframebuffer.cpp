#include "ibl/mipmapcubemapframebuffer.h"

MipmapCubemapFramebuffer::MipmapCubemapFramebuffer(int width, int height) : width(width), height(height) {
    // framebuffer
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // depth buffer
    glGenRenderbuffers(1, &depthRenderbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    // attach the depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferId);

    // cubemap
    glGenTextures(1, &cubemapTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureId);

    // specify/allocate each face for the cubemap
    for (auto i = 0; i < 6;  i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB16F,
            width,
            height,
            0,
            GL_RGB,
            GL_FLOAT,
            nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MipmapCubemapFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
}

void MipmapCubemapFramebuffer::setCubeFace(unsigned int index) {
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
        cubemapTextureId,
        0);
}

unsigned int MipmapCubemapFramebuffer::getCubemapTextureId() {
    return cubemapTextureId;
}
