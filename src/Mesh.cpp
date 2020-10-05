#include "Mesh.h"
//----------------------------------------------------------------------------------------



Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures, Material& material)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;
    VAO = 0;
    VBO = 0;
    EBO = 0;
    setup_mesh(); 
}//----------------------------------------------------------------------------------------

void Mesh::draw(Shader shader)
{
    unsigned int diffuse_id = 1;
    unsigned int specular_id = 1;
    unsigned int normals_id = 1;
    unsigned int height_id = 1;
	
    shader.use();
    shader.setBool("has_texture", !textures.empty()); 
    shader.setVec3("material.ambient", material.Ambient);
    shader.setVec3("material.diffuse", material.Diffuse);
    shader.setVec3("material.specular", material.Specular);
    shader.setFloat("material.shininess", material.Shininess);
    shader.setFloat("material.alpha", material.Alpha);

	// TODO: Obsluga materialow na podstawie tekstur map
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
       /*
        string number;
        string name = textures[i].type;
        if (name == "diffuse")
            number = std::to_string(diffuse_id++);
        else if (name == "texture_specular")
            number = std::to_string(specular_id++); // transfer unsigned int to stream
        else if (name == "texture_normals")
            number = std::to_string(normals_id++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(height_id++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.getId(), ("MaterialMap." + name).c_str()), i);
        */
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}//----------------------------------------------------------------------------------------

void Mesh::setup_mesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // load data into vertex buffers
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // set the vertex attribute pointers
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    glBindVertexArray(0);
}//----------------------------------------------------------------------------------------


