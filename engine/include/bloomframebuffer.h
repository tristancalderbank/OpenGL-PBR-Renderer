#pragma once

#include <iostream>

class BloomFramebuffer {
public:
    BloomFramebuffer(int width, int height);
    void init();
    void bind();
    void resize(int width, int height);
    void setMipLevel(int mipLevel);
    int getMipLevel();
    unsigned int getFramebufferId();
    unsigned int getColorTextureId();

private:
	int mWidth, mHeight;
    int mMipLevel = 0;
	unsigned int mFramebufferId;
	unsigned int mColorTextureId;
};
