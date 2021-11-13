#pragma once

/**
 * A texture that has been loaded in video memory.
 */
struct Texture {
    unsigned int mId;
    std::string mPath; // used to de-dupe textures loaded
};