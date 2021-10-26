#pragma once

#include <glad/glad.h>
#include <iostream>

/**
 * Framebuffer for rendering to faces of a cubemap. The faces themselves are mipmapped.
 */
class MipmapCubemapFramebuffer {
public:
    MipmapCubemapFramebuffer(int width, int height);
    void bind();

    /**
     * Set which cube face texture to render to.
     * @param index
     */
    void setCubeFace(unsigned int index);

    unsigned int getCubemapTextureId();

private:
	int width, height;
	unsigned int framebufferId;
    unsigned int depthRenderbufferId;
	unsigned int cubemapTextureId;
};
