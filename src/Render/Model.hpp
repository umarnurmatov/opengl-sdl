#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <map>

#include "Mesh.hpp"
#include "TextureLoad.hpp"
#include "Filesystem.hpp"


namespace Engine
{

class Model 
{
    public:
        Model(std::string path)
        {
            loadModel(path);
        }
        void draw(Shader& shader);	

    private:
        std::vector<Mesh> meshes;
        std::map<std::string, Texture> loadedTextures;
        std::string modelDirectoryPath;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        void processMesh(Mesh &mesh, aiMesh *mesh_ai, const aiScene *scene);
        void loadMaterialTextures(std::vector<Texture*> &textures, aiMaterial *mat, aiTextureType type);
};

};