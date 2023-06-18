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
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for(size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        auto aiVector3ToGLM = [](aiVector3D &v) -> glm::vec3 { return glm::vec3(v.x, v.y, v.z); };
        v.pos = aiVector3ToGLM(mesh->mVertices[i]);
        v.normal = aiVector3ToGLM(mesh->mNormals[i]);

        if(mesh->mTextureCoords[0])
            v.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            v.texCoords = glm::vec2(0.0f);

        vertices.push_back(v);
    }

    for(size_t i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMap, specularMap;
        loadMaterialTextures(diffuseMap, material, aiTextureType_DIFFUSE);
        loadMaterialTextures(specularMap, material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());
        textures.insert(textures.end(), specularMap.begin(), specularMap.end());
    }

    return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(std::vector<Texture> &textures, aiMaterial *material, aiTextureType textureType)
{
    for(size_t i = 0; i < material->GetTextureCount(textureType); i++)
    {
        Texture texture;
        aiString textureRelativePath_aiString;
        material->GetTexture(textureType, i, &textureRelativePath_aiString);
        std::string textureRelativePath = textureRelativePath_aiString.C_Str();
        // std::cout << str.C_Str() << std::endl;

        if(loadedTextures.contains(textureRelativePath))
        {
            textures.push_back(loadedTextures[textureRelativePath]);
            return;
        }

        texture.id = loadTexture(modelDirectoryPath + textureRelativePath);
        texture.type = textureType == aiTextureType_DIFFUSE ? Texture::DIFFUSE : Texture::SPECULAR;
        texture.path = textureRelativePath;
        textures.push_back(texture);
        loadedTextures[textureRelativePath] = texture;
    }
}

};