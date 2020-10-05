#pragma once
#ifndef LOADER_H
#define LOADER_H
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <vector>
//----------------------------------------------------------------------------------------


class Loader
{
public:
	unsigned int loadTexture(const char* path, const std::string& directory = "", bool gamma = false); // wczytuje teksture
	unsigned int loadCubemap(std::vector<std::string> faces); // wczytuje cubemape
};
#endif