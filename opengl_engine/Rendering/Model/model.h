#pragma once
#include "mesh.h"
#include <Rendering/material.h>
#include <unordered_map>
#include <vector>


class Model
{
public:
	Model(std::vector<LoadMeshData> loadDatas, Material* fillMaterial);
	Model(const Model& other);
	Model& operator=(const Model&) = delete;
	~Model() {}

	/** Change a default material of the model. Note: This doesn't affect the components that already use this model. */
	void changeDefaultMaterial(int materialId, Material* newMaterial);

	/** Check if the given material index exists on this model. */
	bool doesMaterialIndexExists(int materialId) const;

private:
	std::unordered_map<int, std::vector<Mesh>> meshes;
	std::unordered_map<int, Material*> defaultMaterials;
};