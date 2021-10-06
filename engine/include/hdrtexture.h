#pragma once

#include <string>

class HDRTexture {
public:
    HDRTexture(const std::string &path);
    unsigned int getId() {
        return id;
    };
private:
    unsigned int id;
};