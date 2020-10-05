#pragma once
#ifndef CAMERA_H
#define CAMERA_H
//----------------------------------------------------------------------------------------
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
//----------------------------------------------------------------------------------------



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	DOWN,
	UP,
	BONUS,
    CHANGE_RENDER
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{

private:

    void update_camera(); // Calculates the front vector from the Camera's (updated) Euler Angles
	
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // Euler Angles
    float yaw_angle;
    float pitch_angle;
    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom_level;


    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    glm::mat4 get_view_matrix();
	
    void keyboard(Camera_Movement direction, float deltaTime);
    void mouse_movement(float x_offset, float y_offset, GLboolean constrainPitch = true);
    void mouse_scroll(float x_offset, float y_offset);


};
#endif