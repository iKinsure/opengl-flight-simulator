#pragma once
#ifndef MODEL_H
#define MODEL_H
//----------------------------------------------------------------------------------------
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"
#include <string>
#include <iostream>
#include <vector>
#include "Loader.h"
//----------------------------------------------------------------------------------------



using namespace std;

class Model
{
private:

    vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    Loader loader;
	
    void load_model(string const& path); // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void process_node(aiNode* node, const aiScene* scene);  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string typeName); // checks all material textures of a given type and loads the textures if they're not loaded yet, the required info is returned as a Texture struct.
	
public:

    Model(string const& path, bool gamma = false); // constructor, expects a filepath to a 3D model.
    void draw(Shader shader); // draws the model, and thus all its meshes

};
#endif