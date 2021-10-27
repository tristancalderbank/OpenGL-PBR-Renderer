#pragma once

#include <glad/glad.h>
#include <iostream>

/**
 * Framebuffer for rendering to faces of a cubemap. The faces themselves are mipmapped.
 */
class MipmapCubemapFramebuffer {
public:
    MipmapCubemapFramebuffer(unsigned int width, unsigned int height);
    void bind();

    /**
     * Set the mip level to render with.
     * @param mipLevel
     */
    void setMipLevel(unsigned int level);

    /**
     * Get the current width based on the mip level.
     * @return
     */
    unsigned int getWidth();

    /**
     * Get the current height based on the mip level.
     * @return
     */
    unsigned int getHeight();

    /**
     * Set which cube face texture to render to.
     * @param index
     */
    void setCubeFace(unsigned int faceIndex);

    unsigned int getCubemapTextureId();

private:
	unsigned int width, height;
	unsigned int mipWidth, mipHeight;
    unsigned int mipLevel;

	unsigned int framebufferId;
    unsigned int depthRenderbufferId;
	unsigned int cubemapTextureId;
};
