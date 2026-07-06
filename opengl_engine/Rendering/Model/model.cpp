#include "model.h"
#include <ServiceLocator/locator.h>


Model::Model(std::vector<LoadMeshData> loadDatas, Material* fillMaterial)
{
	for (auto& mesh_data : loadDatas)
	{
		meshes[mesh_data.matId].emplace_back(mesh_data);
		defaultMaterials[mesh_data.matId] = fillMaterial;
	}
}

Model::Model(const Model& other) : meshes(other.meshes), defaultMaterials(other.defaultMaterials)
{
}

void Model::changeDefaultMaterial(int materialId, Material* newMaterial)
{
	if (!doesMaterialIndexExists(materialId))
	{
		Locator::getLog().LogMessage_Category("Model: Tried to change the default material of a material index that doesn't exist on this model.", LogCategory::Warning);
		return;
	}

	defaultMaterials[materialId] = newMaterial;
}

bool Model::doesMaterialIndexExists(int materialId) const
{
	return meshes.find(materialId) != meshes.end();
}