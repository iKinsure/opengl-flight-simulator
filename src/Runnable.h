#pragma once
#ifndef RUNNABLE_H
#define RUNNABLE_H
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Globals.h"
//----------------------------------------------------------------------------------------



class Runnable
{
private:

	GLFWwindow* window_;
	
public:

	Runnable(GLFWwindow* window);
	void settings();
	void pbr();
	void out_render_commands();
	void in_render_commands();
	GLFWwindow* get_window();
	void config_shader(Shader& shader);
};




#endif
