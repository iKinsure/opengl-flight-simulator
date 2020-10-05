#include "Runnable.h"
//----------------------------------------------------------------------------------------

Runnable::Runnable(GLFWwindow* window)
{
    this->window_ = window;
}


void Runnable::settings()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_CULL_FACE);

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}//----------------------------------------------------------------------------------------



// opcje renderowania
void Runnable::in_render_commands()
{
    const auto current_frame = float(glfwGetTime()); // per-frame time logic
    DELTA_TIME = current_frame - LAST_FRAME;
    LAST_FRAME = current_frame;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}//----------------------------------------------------------------------------------------




// opcje renderowania
void Runnable::out_render_commands()
{
    glfwSwapBuffers(window_);
    glfwPollEvents();
}//----------------------------------------------------------------------------------------



GLFWwindow* Runnable::get_window()
{
    return window_;
}//----------------------------------------------------------------------------------------


void Runnable::config_shader(Shader &shader)
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(glm::vec3(30.5f, -38.5f, 42.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
	
    shader.use();
    shader.setVec3("viewPos", camera.position);
    shader.setMat4("projection", glm::perspective(glm::radians(camera.zoom_level), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f));
    shader.setMat4("view", camera.get_view_matrix());
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setInt("shadowMap", 1);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setVec3("lightPos", glm::vec3(30.5f, -38.5f, 42.0f));



        // lights
    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f,  10.0f, -5.0f),
        glm::vec3(10.0f,  25.0f, 10.0f),
        glm::vec3(-10.0f, 25.0f, 10.0f),
        glm::vec3(20.0f, 25.0f, 20.0f)
    };
    lightPositions[0] = glm::vec3(3 * glm::sin(glfwGetTime()), 10.0f, -5.0f + 3 * glm::cos(glfwGetTime()));
    lightPositions[1] = glm::vec3(3 * glm::sin(glfwGetTime()), 25.0f, 3 * glm::cos(glfwGetTime()));
    lightPositions[2] = glm::vec3(3 * glm::sin(glfwGetTime()), 20.0f, 1.0f + 3 * glm::cos(glfwGetTime()));
    lightPositions[3] = glm::vec3(20.0f + glm::sin(glfwGetTime()), 25.0f, glm::cos(glfwGetTime()));
    shader.setFloat("materialMap.shininess", 32.0f);
    // directional light
    shader.setVec3("dirLight.direction", 30.5f, -38.5f, 42.0f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);;
    //shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    //shader.setVec3("dirLight.specular", 0.9f, 0.9f, 0.9f);

    // point light 1
    
    shader.setVec3("pointLights[0].position", lightPositions[0]);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    shader.setVec3("pointLights[1].position", lightPositions[1]);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09f);
    shader.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    shader.setVec3("pointLights[2].position", lightPositions[2]);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09f);
    shader.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    shader.setVec3("pointLights[3].position", lightPositions[3]);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09f);
    shader.setFloat("pointLights[3].quadratic", 0.032f);
    
        /*
    glm::vec3 reset = glm::vec3(0.0f, 0.0f, 0.0f);
    shader.setVec3("pointLights[0].position", lightPositions[0]);
    shader.setVec3("pointLights[0].ambient", reset);
    shader.setVec3("pointLights[0].diffuse", reset);
    shader.setVec3("pointLights[0].specular", reset);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.0f);
    shader.setFloat("pointLights[0].quadratic", 0.0f);
    // point light 2
    shader.setVec3("pointLights[1].position", lightPositions[1]);
    shader.setVec3("pointLights[1].ambient", reset);
    shader.setVec3("pointLights[1].diffuse", reset);
    shader.setVec3("pointLights[1].specular", reset);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.0f);
    shader.setFloat("pointLights[1].quadratic", 0.0f);
    // point light 3
    shader.setVec3("pointLights[2].position", lightPositions[2]);
    shader.setVec3("pointLights[2].ambient", reset);
    shader.setVec3("pointLights[2].diffuse", reset);
    shader.setVec3("pointLights[2].specular", reset);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.0f);
    shader.setFloat("pointLights[2].quadratic", 0.0f);
    // point light 4
    shader.setVec3("pointLights[3].position", lightPositions[3]);
    shader.setVec3("pointLights[3].ambient", reset);
    shader.setVec3("pointLights[3].diffuse", reset);
    shader.setVec3("pointLights[3].specular", reset);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.0f);
    shader.setFloat("pointLights[3].quadratic", 0.0f);
    */
    // spotLight
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}//----------------------------------------------------------------------------------------


void Runnable::pbr()
{







	
}
