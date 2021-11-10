#pragma once

#include <iostream>

class Framebuffer {
public:
    Framebuffer(int width, int height);
    void init();
    void resize(int width, int height);
    unsigned int getFramebufferId();
    unsigned int getColorTextureId();
    unsigned int getBloomColorTextureId();

private:
	int width, height;
	unsigned int framebuffer;
	unsigned int colorTexture;
	unsigned int bloomColorTexture;
	unsigned int depthStencilRenderbuffer;
};
