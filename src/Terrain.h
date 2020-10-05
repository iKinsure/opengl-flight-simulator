#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H
//----------------------------------------------------------------------------------------
#include "Model.h"
#include "Shader.h"
//----------------------------------------------------------------------------------------

class Terrain
{
private:
	
	Model terrain;


public:
	
	Terrain();
	void draw(Shader& shader);
	
};




#endif
