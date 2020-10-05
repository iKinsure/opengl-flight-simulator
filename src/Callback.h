#pragma once
#ifndef CALLBACK_H
#define CALLBACK_H
//----------------------------------------------------------------------------------------
#include "Globals.h"
#include <GLFW/glfw3.h>
//----------------------------------------------------------------------------------------


class Callback
{
public:
	
	// callbacks
    static void frame_buffer_size_callback(GLFWwindow* window, int width, int height); // this method is specified as glfw callback
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position); // this method is specified as glfw callback
    static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset); // this method is specified as glfw callback
    static void error_callback(int error, const char* description);

	// callbacks implementations
    void frame_buffer_size_callback_implementation(GLFWwindow* window, int width, int height); // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    void mouse_callback_implementation(GLFWwindow* window, double x_position, double y_position); // glfw: whenever the mouse moves, this callback is called
    void scroll_callback_implementation(GLFWwindow* window, double x_offset, double y_offset); // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    void error_callback_implementation(int error, const char* description); // glfw: callback bledu


	
    static Callback& getInstance() // Singleton is accessed via getInstance(), auto& callbacks = Callback::getInstance();
    {
        static Callback instance; // lazy singleton, instantiated on first use
        return instance;
    }
	
private:
	
    Callback() {}// private constructor necessary to allow only 1 instance
    Callback(Callback const&) {} // prevent copies
    void operator=(Callback const&) {} // prevent assignments

};

#endif