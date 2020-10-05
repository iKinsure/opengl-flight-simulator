#pragma once
#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#define _CRT_SECURE_NO_WARNINGS // NIE RUSZAC!
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Callback.h"
//----------------------------------------------------------------------------------------

class Initialization
{
private:
	
	GLFWwindow* window_; // glowne okno programu
	
	int height_; // wysokosc okna
	int width_; // szerokosc okna
	
public:

	Initialization(const int width, const int height); // konstruktor
	~Initialization(); // destruktor
	
	void initialize_glfw(const char* title); // inicjacja glfw
	void initialize_glad(); // inicjacja glew
	
	void set_callbacks();
	GLFWwindow *get_window();

};

#endif