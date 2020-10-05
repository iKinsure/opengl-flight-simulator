#define _CRT_SECURE_NO_WARNINGS // NIE RUSZAC!
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Input.h"
#include "Globals.h"
#include "Shader.h"
#include "Model.h"
#include "Initialization.h"
#include "Printer.h"
#include "Helicopter.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Runnable.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//----------------------------------------------------------------------------------------



int main()
{
	// inicjacja
    Initialization init(SCREEN_WIDTH, SCREEN_HEIGHT);
    init.initialize_glfw("OpenGL project");
    init.initialize_glad();
    init.set_callbacks();
    Runnable instance(init.get_window());
    instance.settings();
    Shader model_shader("Shaders/model.vs", "Shaders/model.fs");
    Shader text_shader("Shaders/text.vs", "Shaders/text.fs");
    Shader skybox_shader("Shaders/skybox.vs", "Shaders/skybox.fs");
    Helicopter helicopter;
    Terrain terrain;
    Printer printer("resources/fonts/arial.ttf");
    Skybox skybox;
    printer.config(text_shader);
    Model drone("resources/objects/drone/Drone_obj.obj");
    Model nanosuit("resources/objects/nanosuit_reflection/nanosuit.obj");
    Model rock("resources/objects/rock/rock.obj");

    

    // todo:
    Shader pbrShader("Shaders/pbr.vs", "Shaders/pbr.fs");
    Shader equirectangularToCubemapShader("Shaders/cubemap.vs", "Shaders/equirectangular_to_cubemap.fs");
    Shader irradianceShader("Shaders/cubemap.vs", "Shaders/irradiance_convolution.fs");
    Shader prefilterShader("Shaders/cubemap.vs", "Shaders/prefilter.fs");
    Shader brdfShader("Shaders/brdf.vs", "Shaders/brdf.fs");
    Shader backgroundShader("Shaders/background.vs", "Shaders/background.fs");
    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);
    pbrShader.setInt("prefilterMap", 1);
    pbrShader.setInt("brdfLUT", 2);
    pbrShader.setInt("albedoMap", 3);
    pbrShader.setInt("normalMap", 4);
    pbrShader.setInt("metallicMap", 5);
    pbrShader.setInt("roughnessMap", 6);
    pbrShader.setInt("aoMap", 7);
    backgroundShader.use();
    backgroundShader.setInt("environmentMap", 0);
    // load PBR material textures
    // --------------------------
    // rusted iron
    Loader load;
    unsigned int ironAlbedoMap = load.loadTexture("resources/textures/pbr/rusted_iron/albedo.png");
    unsigned int ironNormalMap = load.loadTexture("resources/textures/pbr/rusted_iron/normal.png");
    unsigned int ironMetallicMap = load.loadTexture("resources/textures/pbr/rusted_iron/metallic.png");
    unsigned int ironRoughnessMap = load.loadTexture("resources/textures/pbr/rusted_iron/roughness.png");
    unsigned int ironAOMap = load.loadTexture("resources/textures/pbr/rusted_iron/ao.png");

    // gold
    unsigned int goldAlbedoMap = load.loadTexture("resources/textures/pbr/gold/albedo.png");
    unsigned int goldNormalMap = load.loadTexture("resources/textures/pbr/gold/normal.png");
    unsigned int goldMetallicMap = load.loadTexture("resources/textures/pbr/gold/metallic.png");
    unsigned int goldRoughnessMap = load.loadTexture("resources/textures/pbr/gold/roughness.png");
    unsigned int goldAOMap = load.loadTexture("resources/textures/pbr/gold/ao.png");

    // grass
    unsigned int grassAlbedoMap = load.loadTexture("resources/textures/pbr/grass/albedo.png");
    unsigned int grassNormalMap = load.loadTexture("resources/textures/pbr/grass/normal.png");
    unsigned int grassMetallicMap = load.loadTexture("resources/textures/pbr/grass/metallic.png");
    unsigned int grassRoughnessMap = load.loadTexture("resources/textures/pbr/grass/roughness.png");
    unsigned int grassAOMap = load.loadTexture("resources/textures/pbr/grass/ao.png");

    // plastic
    unsigned int plasticAlbedoMap = load.loadTexture("resources/textures/pbr/plastic/albedo.png");
    unsigned int plasticNormalMap = load.loadTexture("resources/textures/pbr/plastic/normal.png");
    unsigned int plasticMetallicMap = load.loadTexture("resources/textures/pbr/plastic/metallic.png");
    unsigned int plasticRoughnessMap = load.loadTexture("resources/textures/pbr/plastic/roughness.png");
    unsigned int plasticAOMap = load.loadTexture("resources/textures/pbr/plastic/ao.png");

    // wall
    unsigned int wallAlbedoMap = load.loadTexture("resources/textures/pbr/wall/albedo.png");
    unsigned int wallNormalMap = load.loadTexture("resources/textures/pbr/wall/normal.png");
    unsigned int wallMetallicMap = load.loadTexture("resources/textures/pbr/wall/metallic.png");
    unsigned int wallRoughnessMap = load.loadTexture("resources/textures/pbr/wall/roughness.png");
    unsigned int wallAOMap = load.loadTexture("resources/textures/pbr/wall/ao.png");


    // lights
// ------
    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f,  10.0f, -5.0f),
        glm::vec3(10.0f,  25.0f, 10.0f),
        glm::vec3(-10.0f, 25.0f, 10.0f),
        glm::vec3(20.0f, 25.0f, 20.0f)
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    // pbr: setup framebuffer
	// ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


    // pbr: load the HDR environment map
    // ---------------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf("resources/textures/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }
    // pbr: setup cubemap to render to and attach to framebuffer
// ---------------------------------------------------------
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    // pbr: convert HDR equirectangular environment map to cubemap equivalent
   // ----------------------------------------------------------------------
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
 // --------------------------------------------------------------------------------
    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // pbr: generate a 2D LUT from the BRDF equations used.
    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);
    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // initialize static shader uniforms before rendering
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_level), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    pbrShader.use();
    pbrShader.setMat4("projection", projection);
    backgroundShader.use();
    backgroundShader.setMat4("projection", projection);
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(instance.get_window(), &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);





	
		
    while (!glfwWindowShouldClose(instance.get_window()))
    {
        instance.in_render_commands();
        instance.config_shader(model_shader);

    	
        Input::process_input_settings(instance.get_window());
    	if (CAMERA_MODE == FREE_CAMERA) { Input::process_input_free_camera(instance.get_window()); }
        else if (CAMERA_MODE == HELICOPTER_CAMERA) { helicopter.process_input(instance.get_window(), DELTA_TIME); }
        

        // render scene, supplying the convoluted irradiance map to the final shader.
// ------------------------------------------------------------------------------------------
        pbrShader.use();
        pbrShader.setBool("is_light", false);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.get_view_matrix();
        pbrShader.setMat4("view", view);
        pbrShader.setVec3("camPos", camera.position);
        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        // rusted iron
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ironAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ironNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, ironMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, ironRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, ironAOMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0, 15.0, 2.0));
        pbrShader.setMat4("model", model);
        renderSphere();
        // gold
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, goldNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, goldAOMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0, 15.0, 2.0));
        pbrShader.setMat4("model", model);
        renderSphere();
        // grass
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, grassAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, grassNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, grassMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, grassRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, grassAOMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0, 15.0, 2.0));
        pbrShader.setMat4("model", model);
        renderSphere();
        // plastic
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, plasticAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, plasticNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, plasticMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, plasticRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, plasticAOMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0, 15.0, 2.0));
        pbrShader.setMat4("model", model);
        renderSphere();
        // wall
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, wallAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, wallNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, wallMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, wallRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, wallAOMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0, 15.0, 2.0));
        pbrShader.setMat4("model", model);
        renderSphere();
        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        // keeps the codeprint small.
        // gold
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, goldNormalMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, goldAOMap);
        pbrShader.setBool("is_light", true);
        lightPositions[0] = glm::vec3(3*glm::sin(glfwGetTime()), 10.0f, -5.0f + 3*glm::cos(glfwGetTime()));
        lightPositions[1] = glm::vec3(3*glm::sin(glfwGetTime()), 25.0f, 3*glm::cos(glfwGetTime()));
        lightPositions[2] = glm::vec3(3*glm::sin(glfwGetTime()), 20.0f, 1.0f + 3*glm::cos(glfwGetTime()));
        lightPositions[3] = glm::vec3(20.0f + glm::sin(glfwGetTime()), 25.0f, glm::cos(glfwGetTime()));
    	
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            
            glm::vec3 newPos = lightPositions[i];
            pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.1f));
            pbrShader.setMat4("model", model);
            renderSphere();
        }


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model_shader.use();
        model_shader.setMat4("model", model);
        nanosuit.draw(model_shader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-20.0f, 20.0f, 0.0f));
        model_shader.use();
        model_shader.setMat4("model", model);
        drone.draw(model_shader);
        //rock.draw(model_shader);
        
    	
        // helikopter
        helicopter.draw(model_shader);
    	
        // rysowanie planszy
        terrain.draw(model_shader);
    	
    	// skybox
        skybox.prepare();
        skybox.draw(skybox_shader);
        skybox.restore();



    	
    	// tekst
        printer.prepare();
        printer.draw(text_shader, to_string(1.0f / DELTA_TIME), 5.0f, float(SCREEN_HEIGHT) - 20.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
        printer.draw(text_shader, "X = " + to_string(camera.position.x) + " Y = " + to_string(camera.position.y) + " Z = " + to_string(camera.position.z), 5.0f, 5.0f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
        printer.restore();
    	
    	instance.out_render_commands(); // event handler i podwojne buforowanie
    }
    return 0;
}






