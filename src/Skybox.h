#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <string>
#include <vector>
#include "Globals.h"
#include "Loader.h"
#include "Shader.h"

//----------------------------------------------------------------------------------------

class Skybox
{
private:

	
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	unsigned int cubemapTexture;
	
public:
	
	Skybox(); // konstruktor
	void draw(Shader& shader); // rysowanie
	void prepare(); // aktywacja funkcji do rysowania skyboxa
	void restore(); // przywracanie domyslnych ustawien
	unsigned int get_texture();
	
};

#endif