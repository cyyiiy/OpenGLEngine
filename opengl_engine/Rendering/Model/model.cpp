#include "model.h"
#include <Assets/Loaders/modelLoader.h>
#include <Utils/memoryUtils.h>


Model::Model(std::vector<Mesh> _meshes, std::vector<std::shared_ptr<Material>> _defaultMaterials) :
	meshes(std::move(_meshes)), defaultMaterials(std::move(_defaultMaterials))
{
	meshIdsByMaterial.resize(defaultMaterials.size());

	for (size_t id = 0; id < meshes.size(); id++)
	{
		const size_t mat_id = static_cast<size_t>(meshes[id].getMaterialIndex());
		if (mat_id >= meshIdsByMaterial.size())
		{
			throw std::exception("Out of bound MaterialID on a Mesh while constructing a Model.");
		}

		meshIdsByMaterial[mat_id].push_back(id);
	}
}

Model::~Model()
{
	meshes.clear();
	defaultMaterials.clear();
	meshIdsByMaterial.clear();
}

std::string Model::GetTypeName()
{
	return "Model";
}

std::shared_ptr<Model> Model::Create(const LoadParams& params)
{
	if (const auto* file_params = std::get_if<FileImportParams>(&params))
	{
		return ModelLoader::LoadModel(file_params->modelPath, file_params->materials);
	}

	if (const auto* raw_params = std::get_if<RawVerticesParams>(&params))
	{
		std::vector<Mesh> mesh = { Mesh(raw_params->meshVerticesData) };
		std::vector<std::shared_ptr<Material>> material = { raw_params->material };
		return std::make_shared<Model>(mesh, material);
	}
}

Model::LoadParams Model::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}

uint64_t Model::getAssetMemorySize() const
{
	uint64_t total = sizeof(Model);
	total += MemoryUtils::EstimateVectorHeapMemory(meshes);
	total += MemoryUtils::EstimateVectorHeapMemory(defaultMaterials);
	total += MemoryUtils::EstimateVectorHeapMemory(meshIdsByMaterial);
	return total;
}

uint64_t Model::getAssetGpuSize() const
{
	uint64_t total = 0;
	for (const Mesh& mesh : meshes)
	{
		total += mesh.getVertexArray().getVertexArrayGPUMemory();
	}
	return total;
}

const std::vector<size_t>& Model::getMeshIdsForMaterialId(MaterialID id) const
{
	return meshIdsByMaterial[static_cast<size_t>(id)];
}
