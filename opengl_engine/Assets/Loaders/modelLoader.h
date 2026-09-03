#pragma once
#include <Rendering/Model/model.h>
#include <Rendering/material.h>
#include <filesystem>
#include <vector>
#include <memory>


namespace ModelLoader
{
	/** Load a model from a 3D file using assimp. */
	std::shared_ptr<Model> LoadModel(const std::filesystem::path& modelPath, const std::vector<std::shared_ptr<Material>>& defaultMaterials);
};