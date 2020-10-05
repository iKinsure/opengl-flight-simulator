#include "Camera.h"
//----------------------------------------------------------------------------------------



// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom_level(ZOOM)
{
    this->position = position;
    this->world_up = up;
    yaw_angle = yaw;
    pitch_angle = pitch;
    update_camera();
}//---------------------------------------------------------------------------------------



// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position + front, this->up);
}//----------------------------------------------------------------------------------------


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::keyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD)
        position += velocity * front;
    if (direction == BACKWARD)
        position -= velocity * front;
    if (direction == LEFT)
        position -= velocity * this->right;
    if (direction == RIGHT)
        position += velocity * this->right;
    if (direction == DOWN)
        position -= velocity * this->up;
    if (direction == UP)
        position += velocity * this->up;
}//----------------------------------------------------------------------------------------


// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::mouse_movement(float x_offset, float y_offset, GLboolean constrainPitch)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw_angle += x_offset;
    pitch_angle += y_offset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch_angle > 89.0f)
            pitch_angle = 89.0f;
        if (pitch_angle < -89.0f)
            pitch_angle = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    update_camera();
}//----------------------------------------------------------------------------------------


// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::mouse_scroll(float x_offset, float y_offset)
{
    if (zoom_level >= 1.0f && zoom_level <= 90.0f)
        zoom_level -= y_offset;
    if (zoom_level <= 1.0f)
        zoom_level = 1.0f;
    if (zoom_level >= 90.0f)
        zoom_level = 90.0f;
}//----------------------------------------------------------------------------------------


// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::update_camera()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_angle)) * cos(glm::radians(pitch_angle));
    front.y = sin(glm::radians(pitch_angle));
    front.z = sin(glm::radians(yaw_angle)) * cos(glm::radians(pitch_angle));
    this->front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->world_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}//----------------------------------------------------------------------------------------