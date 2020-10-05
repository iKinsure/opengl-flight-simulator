#pragma once
#ifndef HELICOPTER_H
#define HELICOPTER_H


//----------------------------------------------------------------------------------------

#include "Model.h"
#include <GLFW/glfw3.h>
#include "Globals.h"

//----------------------------------------------------------------------------------------




class Helicopter
{
private:
	
	Model helicopter_main;
	Model helicopter_propeller;
	
	glm::mat4 model;
	glm::mat4 reset;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;
	float yaw_angle;
	float pitch_angle;
	float movement_speed;
	float velocity;
	
public:
	
	Helicopter();
	void draw(Shader& shader);
	void process_input(GLFWwindow* window, float deltaTime);
	
};

#endif