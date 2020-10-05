#include "Helicopter.h"




//----------------------------------------------------------------------------------------


Helicopter::Helicopter()
: helicopter_main("resources/objects/helicopter/main/Heli_bell.obj")
, helicopter_propeller("resources/objects/helicopter/propeller1/Heli_bell.obj")
{

    movement_speed = 2.5f;
    model = glm::mat4(1.0f);
    reset = glm::mat4(1.0f);
    yaw_angle = 0.0f;
    pitch_angle = 0.0f;
    this->position = camera.position;
    this->world_up = camera.world_up;
    this->front = camera.front;
	
}

void Helicopter::draw(Shader &shader)
{
    glm::vec3 front;
    front.x = sin(glm::radians(yaw_angle));
    front.y = 0.0f;
    front.z = cos(glm::radians(yaw_angle));
    this->front = glm::normalize(front);
    if (CAMERA_MODE == 1)
    {
        camera.position = this->position + glm::vec3(-5.0f * this->front.x, 3.0f, -5.0f * this->front.z);
        shader.setVec3("spotLight.position", this->position);
        shader.setVec3("spotLight.direction", camera.front);
    }
    else
    {
        //shader.setVec3("spotLight.position", camera.position + glm::vec3(1.0f, -3.0f, 3.5f));
        //shader.setVec3("spotLight.direction", camera.front);
    }
    model = reset;
    model = glm::translate(model, this->position);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    model = glm::rotate(model, glm::radians(yaw_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    helicopter_main.draw(shader);
    model = glm::rotate(model, glm::radians(1000.0f * float(glfwGetTime())), glm::vec3(0.0, 1.0f, 0.0));
    shader.setMat4("model", model);
    helicopter_propeller.draw(shader);
}


void Helicopter::process_input(GLFWwindow* window, float deltaTime)
{
    velocity = movement_speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        yaw_angle += 1.0f;;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        yaw_angle -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += velocity * front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= velocity * front;
        
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position.y += velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        position.y -= velocity;
    }

}//----------------------------------------------------------------------------------------