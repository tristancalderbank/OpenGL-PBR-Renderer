#pragma once

#include <glad/glad.h>
#include <iostream>

class Framebuffer {
public:
    Framebuffer(int width, int height);
    void init();
    void resize(int width, int height);
    unsigned int getFramebufferHandle();
    unsigned int getColorTextureHandle();

private:
	int width, height;
	unsigned int framebuffer;
	unsigned int colorTexture;
	unsigned int depthStencilRenderbuffer;
};
