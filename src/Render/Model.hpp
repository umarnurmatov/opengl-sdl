#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "Mesh.hpp"

class Model 
{
    public:
        /*  Методы   */
        Model(std::string path)
        {
            m_loadModel(path);
        }
        void draw(MyGL::Shader& shader);	
    private:
        /*  Данные модели  */
        std::vector<Mesh> m_meshes;
        std::string m_directory;

        /*  Методы   */
        void m_loadModel(std::string path);
        void m_processNode(aiNode *node, const aiScene *scene);
        Mesh m_processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> m_loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};