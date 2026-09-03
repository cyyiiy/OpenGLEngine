#include "modelLoader.h"
#include <ServiceLocator/locator.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Utils/defines.h>


std::vector<MeshVerticesData> LoadMeshes(const std::filesystem::path& meshesPath)
{
    // 1. Prepare the return vector
    std::vector<MeshVerticesData> meshes_datas;

    // 2. Import the meshes file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(meshesPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

    // 3. Handle errors while importing the file
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Locator::getLog().LogMessage_Category("Assimp_Import: " + std::string(importer.GetErrorString()), LogCategory::Error);
        return {};
    }

    // 4. Recursively process all the meshes, starting from the root node
    ProcessAssimpNode(scene->mRootNode, scene, meshes_datas);
    return meshes_datas;
}

void ProcessAssimpNode(aiNode* node, const aiScene* scene, std::vector<MeshVerticesData>& meshesDatas)
{
    // 1. Process all the meshes of the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshesDatas.push_back(ProcessAssimpMesh(mesh, node, scene));
    }

    // 2. Call this function on each of the node's children to recursively process all the nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessAssimpNode(node->mChildren[i], scene, meshesDatas);
    }
}

MeshVerticesData ProcessAssimpMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
    // 1. Compute the transformation and normal matrix of the node
    aiMatrix4x4 node_matrix = RetrieveAssimpParentTransform(node);
    aiMatrix4x4 node_matrix_normal = node_matrix;
    node_matrix_normal.Inverse();
    node_matrix_normal.Transpose();

    // 2. Prepare the elements of the 'LoadMeshData' struct
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    MaterialID material = static_cast<MaterialID>(mesh->mMaterialIndex);

    // 3. Process the vertices of the mesh
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // a. Prepare the vertex object
        Vertex vertex;

        // b. Process the vertex position
        aiVector3D vertices_transformed = mesh->mVertices[i];
        vertices_transformed *= node_matrix;

        Vector3 pos;
        pos.x = vertices_transformed.x;
        pos.y = vertices_transformed.y;
        pos.z = vertices_transformed.z;
        vertex.position = pos;

        // c. Process the vertex normal
        aiVector3D normal_transformed = mesh->mNormals[i];
        normal_transformed *= node_matrix_normal; // Note: It is supposed to rotate the normal properly if the node has a rotation but I'm not sure it works perfectly

        Vector3 normal;
        normal.x = normal_transformed.x;
        normal.y = normal_transformed.y;
        normal.z = normal_transformed.z;
        vertex.normal = normal;

        // d. Process the vertex texture coordinates
        if (mesh->mTextureCoords[0]) // Note: Handle the case where a vertex doesn't have texture coordinates
        {
            Vector2 tex_coords;
            tex_coords.x = mesh->mTextureCoords[0][i].x;
            tex_coords.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tex_coords;
        }
        else
        {
            vertex.texCoords = Vector2::zero;
        }

        // e. Send the process vertex
        vertices.push_back(vertex);
    }

    // 4. Process the indices of the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 5. Return the processed mesh data
    return MeshVerticesData{ vertices, indices, material };
}

aiMatrix4x4 RetrieveAssimpParentTransform(aiNode* node)
{
    // Recursively multiply the node parent's transformation matrix until the root node
    aiNode* parent = node->mParent;
    if (parent == nullptr) return node->mTransformation;

    return RetrieveAssimpParentTransform(node->mParent) * node->mTransformation;
}

bool AssertMaterialIdsSanity(const std::vector<MeshVerticesData>& meshesDatas, const std::vector<std::shared_ptr<Material>>& materials)
{
    // Check if the material ids of the meshes datas are in range of the materials list
    const MaterialID max_mat_id = static_cast<MaterialID>(materials.size());

    for (const MeshVerticesData& mesh_data : meshesDatas)
    {
        if (mesh_data.matId >= max_mat_id) return false;
    }

    return true;
}


std::shared_ptr<Model> ModelLoader::LoadModel(const std::filesystem::path& modelPath, const std::vector<std::shared_ptr<Material>>& defaultMaterials)
{
    // 1. Resolve the model path from the resource folder
    std::filesystem::path path = RESOURCES_PATH;
    path += modelPath;

    // 2. Load the mesh vertices of the model
    std::vector<MeshVerticesData> meshes_datas = LoadMeshes(path);

    // 3. Ensure the default materials list works with the generated material ids of the meshes 
    if (!AssertMaterialIdsSanity(meshes_datas, defaultMaterials))
    {
        Locator::getLog().LogMessage_Category("Model Loader: Given default materials list does not have the Material IDs corresponding to the loaded model.", LogCategory::Error);
        return nullptr;
    }

    // 4. Construct the meshes list
    std::vector<Mesh> meshes;
    meshes.reserve(meshes_datas.size());

    for (MeshVerticesData& mesh_data : meshes_datas)
    {
        meshes.emplace_back(Mesh(mesh_data));
    }

    // 5. Construct the model object and return it
    return std::make_shared<Model>(meshes, defaultMaterials);
}
