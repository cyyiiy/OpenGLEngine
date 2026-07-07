#include "modelRendererComponent.h"
#include <ServiceLocator/locator.h>


void ModelRendererComponent::setModel(Model* newModel)
{
	model = newModel;
	resetToDefaultMaterials();
}

void ModelRendererComponent::setMaterial(Material* material, int materialId)
{
	// 1. Check if the model used by this component is valid
	if (!isValid())
	{
		Locator::getLog().LogMessage_Category("Model Renderer Component: Tried to change a material on a component that doesn't have a valid model.", LogCategory::Warning);
		return;
	}

	// 2. Check if the given material index exists on the model used by this component
	if (!model->doesMaterialIndexExists(materialId))
	{
		Locator::getLog().LogMessage_Category("Model Renderer Component: Tried to change a material of a material index that doesn't exist on the model of this component.", LogCategory::Warning);
		return;
	}

	// 3. Remove the current occurence of the given material index in the materials list of this component
	for (std::pair<const MaterialUniqueID, std::vector<int>>& pair : materials)
	{
		// a. For each registered material, check if it associated with the given material index
		std::vector<int>& material_ids = pair.second;
		auto iter = std::find(material_ids.begin(), material_ids.end(), materialId);
		if (iter == material_ids.end()) continue;

		// b. If the given material index is the only index associated with a material, remove this material completely
		if (material_ids.size() == 1)
		{
			materials.erase(pair.first);
		}

		// c. If the given material index share a material with other index, just remove the given index
		else
		{
			material_ids.erase(iter);
		}

		// d. There is only one occurence of a material index in the map, so the loop can end as soon as it has been found and managed
		break;
	}

	// 4. Re-add the given material index under the new material
	materials[getMaterialUniqueID(material)].push_back(materialId);
}

void ModelRendererComponent::resetToDefaultMaterials()
{
	materials.clear();
	if (!isValid()) return;

	const std::unordered_map<int, Material*>& default_materials = model->getDefaultMaterials();
	for (const std::pair<int, Material*>& material_pair : default_materials)
	{
		const int material_id = material_pair.first;
		const Material* material = material_pair.second;

		materials[getMaterialUniqueID(material)].push_back(material_id);
	}
}

const std::vector<std::shared_ptr<Mesh>> ModelRendererComponent::retrieveMeshesOfMaterial(const Material& material) const
{
	if (!isValid()) return {};

	if (materials.find(getMaterialUniqueID(&material)) == materials.end()) return {};

	std::vector<std::shared_ptr<Mesh>> meshes_of_material;
	const std::vector<int>& ids_of_material = materials.at(getMaterialUniqueID(&material));
	
	for (int id : ids_of_material)
	{
		const std::vector<std::shared_ptr<Mesh>> meshes = model->getMeshesOfMaterialId(id);
		meshes_of_material.insert(std::end(meshes_of_material), std::begin(meshes), std::end(meshes));
	}

	return meshes_of_material;
}

bool ModelRendererComponent::isValid() const
{
	return model != nullptr;
}

MaterialUniqueID ModelRendererComponent::getMaterialUniqueID(const Material* material) const
{
	// Utility function to prevent crash if checking the material unique id of nullptr
	if (material == nullptr) return 0;

	return material->getUniqueID();
}