#include "modelLoader.h"
#include "meshLoader.h"
#include <Utils/defines.h>


Model ModelLoader::LoadModel(const std::string& modelPath, Material* fillMaterial)
{
    // 1. Create the real path from the resources folder
    const std::string path = RESOURCES_PATH + modelPath;

    // 2. Load the meshes of the model
    std::vector<LoadMeshData> meshes_datas = MeshLoader::LoadMeshes(path);

    // 3. Construct the model object in the return statement
    return Model(meshes_datas, fillMaterial);
}