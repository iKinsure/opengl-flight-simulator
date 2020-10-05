#include "Terrain.h"
//----------------------------------------------------------------------------------------


Terrain::Terrain()
    : terrain("resources/objects/Tropical_island/Small_Tropical_Island.obj")
{
	
}


void Terrain::draw(Shader& shader)
{
    glm::mat4 resize = glm::mat4(1.0f);
    resize = glm::scale(resize, glm::vec3(0.01f, 0.01f, 0.01f));
    const unsigned int size = 4;
    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {
            shader.setMat4("model", resize);
            terrain.draw(shader);
            resize = glm::translate(resize, glm::vec3(0.0f, 0.0f, 710.0f));
        }
        resize = glm::translate(resize, glm::vec3(710.0f, 0.0f, -710.0f * float(size)));
    }
}


