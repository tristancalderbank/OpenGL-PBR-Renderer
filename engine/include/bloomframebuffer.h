#pragma once

#include <iostream>

class BloomFramebuffer {
public:
    BloomFramebuffer(int width, int height);
    void init();
    void bind();
    void resize(int width, int height);
    unsigned int getFramebufferId();
    unsigned int getColorTextureId();

private:
	int mWidth, mHeight;
	unsigned int mFramebufferId;
	unsigned int mColorTextureId;
};
