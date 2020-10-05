#pragma once
#ifndef MESH_H
#define MESH_H
//----------------------------------------------------------------------------------------
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Vertex.h"
//----------------------------------------------------------------------------------------


using namespace std;



struct Texture { 
    unsigned int id;
    string type;
    string path;
};


struct Material
{
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;
    float Alpha;
};

class Mesh {
	
private:

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
    unsigned int VAO;
    unsigned int VBO, EBO;
	
    void setup_mesh(); // initializes all the buffer objects/arrays
	
public:

    Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices, vector<Texture> &textures, Material &material); // constructor
    void draw(Shader shader); // render the mesh


};
#endif