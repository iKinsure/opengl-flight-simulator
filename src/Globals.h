#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
//----------------------------------------------------------------------------------------
#include <glm/detail/type_vec3.hpp>
#include "Camera.h"
#include "Helicopter.h"
//----------------------------------------------------------------------------------------
#define FREE_CAMERA 0
#define HELICOPTER_CAMERA 1


extern Camera camera;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern double X_LAST;
extern double Y_LAST;
extern bool IS_FIRST_MOUSE;
extern float DELTA_TIME;
extern float LAST_FRAME;
extern int CAMERA_MODE;

//----------------------------------------------------------------------------------------
// temporary



extern unsigned int sphereVAO;
extern unsigned int indexCount;
extern void renderSphere();
extern unsigned int cubeVAO;
extern unsigned int cubeVBO;
extern void renderCube();
extern unsigned int quadVAO;
extern unsigned int quadVBO;
extern void renderQuad();

#endif