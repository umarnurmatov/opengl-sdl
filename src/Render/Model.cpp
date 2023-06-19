#include "Model.hpp"


namespace Engine
{

void Model::draw(Shader &shader)
{
    for(auto& m : meshes)
        m.draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) 
    {
        logCritical(importer.GetErrorString());
        return;
    }

    modelDirectoryPath = path.substr(0, path.find_last_of('/')) + '/';

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh_ai = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(Mesh{}); 
        processMesh(meshes[meshes.size()-1], mesh_ai, scene);
    }

    for(size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(Mesh &mesh, aiMesh *mesh_ai, const aiScene *scene)
{
    for(size_t i = 0; i < mesh_ai->mNumVertices; i++)
    {
        Vertex v;
        auto aiVector3ToGLM = [](aiVector3D &v) -> glm::vec3 { return glm::vec3(v.x, v.y, v.z); };
        v.pos = aiVector3ToGLM(mesh_ai->mVertices[i]);
        v.normal = aiVector3ToGLM(mesh_ai->mNormals[i]);

        if(mesh_ai->mTextureCoords != nullptr)
            v.texCoords = glm::vec2(mesh_ai->mTextureCoords[0][i].x, mesh_ai->mTextureCoords[0][i].y);
        else
            v.texCoords = glm::vec2(0.0f);

        mesh.vertices.push_back(v);
    }

    for(size_t i = 0; i < mesh_ai->mNumFaces; i++) 
    {
        aiFace face = mesh_ai->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++)
            mesh.indices.push_back(face.mIndices[j]);
    }

    if(mesh_ai->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh_ai->mMaterialIndex];
        loadMaterialTextures(mesh.textures, material, aiTextureType_DIFFUSE);
        loadMaterialTextures(mesh.textures, material, aiTextureType_SPECULAR);
    }

    mesh.setupMesh();
}

void Model::loadMaterialTextures(std::vector<Texture*> &textures, aiMaterial *material, aiTextureType textureType)
{
    for(size_t i = 0; i < material->GetTextureCount(textureType); i++)
    {
        Texture texture;
        aiString textureRelativePath_aiString;
        material->GetTexture(textureType, i, &textureRelativePath_aiString);
        std::string textureRelativePath = textureRelativePath_aiString.C_Str();

        if(loadedTextures.contains(textureRelativePath))
        {
            textures.push_back(&loadedTextures[textureRelativePath]);
            return;
        }

        texture.id = loadTexture(modelDirectoryPath + textureRelativePath);
        texture.type = textureType == aiTextureType_DIFFUSE ? Texture::DIFFUSE : Texture::SPECULAR;
        loadedTextures[textureRelativePath] = texture;
        textures.push_back(&loadedTextures[textureRelativePath]);
    }
}

};