#pragma once
#ifndef INPUT_H
#define INPUT_H
//----------------------------------------------------------------------------------------
#include "Camera.h"
#include "Globals.h"
//----------------------------------------------------------------------------------------



class Input
{
private:

	Helicopter* helicopter;
	
public:

	Input();
	static void process_input_free_camera(GLFWwindow* window);
	static void process_input_settings(GLFWwindow* window);
	
};




#endif
