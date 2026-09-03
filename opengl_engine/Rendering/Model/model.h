#pragma once
#include <Assets/assetInterface.h>
#include <Rendering/Model/mesh.h>
#include <Rendering/material.h>
#include <unordered_map>
#include <variant>
#include <vector>
#include <memory>


class Model : public IAsset
{
public:
	// Asset part
	struct FileImportParams
	{
		std::filesystem::path modelPath;
		std::vector<std::shared_ptr<Material>> materials;
	};

	struct RawVerticesParams
	{
		MeshVerticesData meshVerticesData;
		std::shared_ptr<Material> material;
	};

	using LoadParams = std::variant<FileImportParams, RawVerticesParams>;


	Model(std::vector<Mesh> _meshes, std::vector<std::shared_ptr<Material>> _defaultMaterials);
	~Model();

	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<Model> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// Model part
	const std::vector<size_t>& getMeshIdsForMaterialId(MaterialID id) const;

	const Mesh& getMesh(size_t id) const { return meshes[id]; }
	const std::vector<Mesh>& getMeshes() const { return meshes; }

	const std::vector<std::shared_ptr<Material>>& getDefaultMaterials() const { return defaultMaterials; }
	size_t getMaterialCount() const { return defaultMaterials.size(); }

private:
	std::vector<Mesh> meshes;
	std::vector<std::shared_ptr<Material>> defaultMaterials;
	std::vector<std::vector<size_t>> meshIdsByMaterial;
};