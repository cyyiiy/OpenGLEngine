#include "model.h"
#include <ServiceLocator/locator.h>


Model::Model(std::vector<LoadMeshData> loadDatas, Material* fillMaterial)
{
	for (auto& mesh_data : loadDatas)
	{
		meshes[mesh_data.matId].emplace_back(std::make_shared<Mesh>(mesh_data));
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

const std::vector<std::shared_ptr<Mesh>> Model::getMeshesOfMaterialId(int materialId) const
{
	if (!doesMaterialIndexExists(materialId))
	{
		Locator::getLog().LogMessage_Category("Model: Tried to get the meshes of a material index that doesn't exist on this model.", LogCategory::Warning);
		return {};
	}

	return meshes.at(materialId);
}

const std::unordered_map<int, Material*>& Model::getDefaultMaterials() const
{
	return defaultMaterials;
}

bool Model::doesMaterialIndexExists(int materialId) const
{
	return meshes.find(materialId) != meshes.end();
}