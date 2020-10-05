#pragma once
#ifndef PRINTER_H
#define PRINTER_H

//----------------------------------------------------------------------------------------
#include <glad/glad.h>

#include <map>
#include <string>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.h"
#include "Globals.h"
//----------------------------------------------------------------------------------------


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};


class Printer
{
private:
	
	unsigned int VAO;
	unsigned int VBO;
	std::map<GLchar, Character> Characters;

	
public:
	
	Printer(const char* filepath);
	void draw(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color); // renderuje tekst
	void prepare(); // konfiguracja do wyswietlania tekstu
	void restore(); // przywrocenie domyslnej konfiguracji
	void config(Shader& shader);
	
};



#endif