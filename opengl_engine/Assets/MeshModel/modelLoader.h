#pragma once
#include <Rendering/Model/model.h>
#include <string>


namespace ModelLoader
{
	/** Load a model from a 3D file using assimp. */
	static Model LoadModel(const std::string& modelPath);
};