#pragma once

/**
 * Framebuffer for rendering brdf convolution map.
 */
class BrdfConvolutionFramebuffer {
public:
    BrdfConvolutionFramebuffer(unsigned int width, unsigned int height);

    /**
     * Activate this framebuffer for drawing.
     */
    void bind();

    /**
     * Get the width.
     * @return
     */
    unsigned int getWidth();

    /**
     * Get the height.
     * @return
     */
    unsigned int getHeight();

    /**
     * Get the GL color texture id.
     * @return
     */
    unsigned int getColorTextureId();

private:
	unsigned int width, height;

	unsigned int framebufferId;
    unsigned int depthRenderbufferId;
	unsigned int colorTextureId;
};
