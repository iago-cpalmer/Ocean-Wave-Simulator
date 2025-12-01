#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <string>
#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

typedef struct
{
    unsigned int TextureId;
} Cubemap;



void load_cubemap(Cubemap *cubemap, std::vector<std::string> faces);

#endif  