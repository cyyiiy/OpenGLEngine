#pragma once
#include <Rendering/Model/model.h>
#include <string>


namespace ModelLoader
{
	/** Load a model from a 3D file using assimp and fill its default material slots with the given material. */
	Model LoadModel(const std::string& modelPath, class Material* fillMaterial);
};