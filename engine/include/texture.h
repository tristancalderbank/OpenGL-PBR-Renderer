#pragma once

struct Texture {
    unsigned int id;
    std::string path; // used to de-dupe textures loaded
};