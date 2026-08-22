#pragma once
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>

#include "shader.h"
#include "texture.h"
#include <Maths/Vector3.h>

#include <unordered_map>
#include <memory>
#include <string>


class Material : public IAsset
{
	// TODO: Create a null material that is a gray-ish albedo (could also serve as a default material?)

public:
	// Asset part
	struct LoadParams
	{
		std::shared_ptr<const Shader> shader;
		std::unordered_map<std::string, std::shared_ptr<const Texture>> textures;

		std::unordered_map<std::string, bool> boolParameters;
		std::unordered_map<std::string, int> intParameters;
		std::unordered_map<std::string, float> floatParameters;
		std::unordered_map<std::string, Vector3> vec3Parameters;

		LoadParams(std::shared_ptr<const Shader> _shader) : shader(_shader) {}

		// Note: For now, it is not possible to ensure a material have the correct textures and parameters to fill its shader.
		// It could be possible in the future with a custom shader builder that explicitly output its parameters.
	};

	Material(LoadParams materialLoadParams);
	~Material();

	Material(const Material&) = delete;
	Material(Material&&) = delete;
	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<Material> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// Material part
	void use() const;
	const std::shared_ptr<const Shader> getShader() const { return shader; }


private:
	std::shared_ptr<const Shader> shader;
	std::unordered_map<std::string, std::shared_ptr<const Texture>> textures;

	std::unordered_map<std::string, bool> boolParameters;
	std::unordered_map<std::string, int> intParameters;
	std::unordered_map<std::string, float> floatParameters;
	std::unordered_map<std::string, Vector3> vector3Parameters;
};