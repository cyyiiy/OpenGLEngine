#pragma once
#include "mesh.h"
#include <Rendering/material.h>
#include <unordered_map>
#include <vector>
#include <memory>


class Model
{
public:
	Model(std::vector<LoadMeshData> loadDatas, Material* fillMaterial);
	Model(const Model& other);
	Model& operator=(const Model&) = delete;
	~Model() {}

	/** Change a default material of the model. Note: This doesn't affect the components that already use this model. */
	void changeDefaultMaterial(int materialId, Material* newMaterial);

	/** Get all the meshes of the model that uses the given material id. */
	const std::vector<std::shared_ptr<Mesh>> getMeshesOfMaterialId(int materialId) const;

	/** Return the default materials of the model. */
	const std::unordered_map<int, Material*>& getDefaultMaterials() const;

	/** Check if the given material index exists on this model. */
	bool doesMaterialIndexExists(int materialId) const;

private:
	std::unordered_map<int, std::vector<std::shared_ptr<Mesh>>> meshes;
	std::unordered_map<int, Material*> defaultMaterials;
};