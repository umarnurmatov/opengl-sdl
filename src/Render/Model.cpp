#include "Model.hpp"

void Model::draw(MyGL::Shader &shader)
{
    for(auto& m : m_meshes)
        m.draw(shader);
}

void Model::m_loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) 
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    m_processNode(scene->mRootNode, scene);
}

void Model::m_processNode(aiNode *node, const aiScene *scene)
{
    for(size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(m_processMesh(mesh, scene));
    }

    for(size_t i = 0; i < node->mNumChildren; i++)
    {
        m_processNode(node->mChildren[i], scene);
    }
}

Mesh Model::m_processMesh(aiMesh *mesh, const aiScene *scene)
{
    // return Mesh();
}

std::vector<Texture> Model::m_loadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
    return std::vector<Texture>();
}
