#pragma once

#include <iostream>

/**
 * The main framebuffer.
 */
class Framebuffer {
public:
    Framebuffer(int width, int height);
    void init();
    void bind();
    void resize(int width, int height);
    unsigned int getFramebufferId();
    unsigned int getColorTextureId();
    unsigned int getBloomColorTextureId();

private:
	int mWidth, mHeight;
	unsigned int mFramebuffer;
	unsigned int mColorTexture;
	unsigned int mBloomColorTexture;
	unsigned int mDepthStencilRenderbuffer;
};
