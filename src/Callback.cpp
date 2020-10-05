#include "Callback.h"
//----------------------------------------------------------------------------------------


// this method is specified as glfw callback
void Callback::frame_buffer_size_callback(GLFWwindow* window, int width, int height) 
{
    getInstance().frame_buffer_size_callback_implementation(window, width, height);
}//----------------------------------------------------------------------------------------
// this method is specified as glfw callback
void Callback::mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
    getInstance().mouse_callback_implementation(window, x_position, y_position);
}//----------------------------------------------------------------------------------------
// this method is specified as glfw callback
void Callback::scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    getInstance().scroll_callback_implementation(window, x_offset, y_offset);
}//----------------------------------------------------------------------------------------
void Callback::error_callback(int error, const char* description)
{
    getInstance().error_callback_implementation(error, description);
}//----------------------------------------------------------------------------------------




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Callback::frame_buffer_size_callback_implementation(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    
    glViewport(0, 0, width, height);
}//----------------------------------------------------------------------------------------


// glfw: whenever the mouse moves, this callback is called
void Callback::mouse_callback_implementation(GLFWwindow* window, double x_position, double y_position)
{
    if (IS_FIRST_MOUSE)
    {
        X_LAST = x_position;
        Y_LAST = y_position;
        IS_FIRST_MOUSE = false;
    }
    const auto x_offset = x_position - X_LAST;
    const auto y_offset = Y_LAST - y_position; // reversed since y-coordinates go from bottom to top
    X_LAST = x_position;
    Y_LAST = y_position;
    camera.mouse_movement(float(x_offset), float(y_offset));
}//----------------------------------------------------------------------------------------


// scroll
void Callback::scroll_callback_implementation(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.mouse_scroll(float(x_offset), float(y_offset));
}//----------------------------------------------------------------------------------------


// error
void Callback::error_callback_implementation(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}//----------------------------------------------------------------------------------------

