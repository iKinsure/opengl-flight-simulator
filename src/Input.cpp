#include "Input.h"
//----------------------------------------------------------------------------------------

Input::Input()
{
    helicopter = nullptr;
}


// implementacja callbacka
void Input::process_input_free_camera(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.keyboard(FORWARD, DELTA_TIME);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.keyboard(BACKWARD, DELTA_TIME);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.keyboard(LEFT, DELTA_TIME);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.keyboard(RIGHT, DELTA_TIME);
    }
}//----------------------------------------------------------------------------------------


void Input::process_input_settings(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        glfwWaitEventsTimeout(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        
        GLint polygon_mode;
        glGetIntegerv(GL_POLYGON_MODE, &polygon_mode);
        if (polygon_mode == GL_LINE)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (polygon_mode == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glfwWaitEventsTimeout(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        if (CAMERA_MODE == 0)
        {
            CAMERA_MODE = 1;
        }
        else
        {
            CAMERA_MODE = 0;
        }
        glfwWaitEventsTimeout(1.0);
    }
}