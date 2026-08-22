#include "material.h"
#include <ServiceLocator/locator.h>
#include <Utils/memoryUtils.h>
#include <glad/glad.h>


Material::Material(LoadParams materialLoadParams) : IAsset(),
	shader(materialLoadParams.shader), textures(materialLoadParams.textures),
	boolParameters(materialLoadParams.boolParameters), intParameters(materialLoadParams.intParameters),
	floatParameters(materialLoadParams.floatParameters), vector3Parameters(materialLoadParams.vec3Parameters)
{
	Locator::getRenderer().AddMaterial(this);
}

Material::~Material()
{
	Locator::getRenderer().RemoveMaterial(this);
}


std::string Material::GetTypeName()
{
	return "Material";
}

std::shared_ptr<Material> Material::Create(const LoadParams& params)
{
	return std::make_shared<Material>(params);
}

Material::LoadParams Material::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}

uint64_t Material::getAssetMemorySize() const
{
	uint64_t total = sizeof(Material);
	total += MemoryUtils::EstimateUnorderedMapHeapMemory(textures);
	total += MemoryUtils::EstimateUnorderedMapHeapMemory(boolParameters);
	total += MemoryUtils::EstimateUnorderedMapHeapMemory(intParameters);
	total += MemoryUtils::EstimateUnorderedMapHeapMemory(floatParameters);
	total += MemoryUtils::EstimateUnorderedMapHeapMemory(vector3Parameters);
	return total;
}

uint64_t Material::getAssetGpuSize() const
{
	return 0;
}


void Material::use() const
{
	if (!shader) return;

	unsigned int tex_activated = 0;
	for (const auto& [name, texture] : textures)
	{
		glActiveTexture(GL_TEXTURE0 + tex_activated); // Activate texture unit first

		shader->setInt("material." + name, tex_activated); // Then set the sampler to the correct texture unit
		texture->use(); // Finally bind the texture

		tex_activated++;
	}

	glActiveTexture(GL_TEXTURE0); // Clear the texture unit activation

	for (auto parameter : boolParameters) shader->setBool(parameter.first, parameter.second);
	for (auto parameter : intParameters) shader->setInt(parameter.first, parameter.second);
	for (auto parameter : floatParameters) shader->setFloat(parameter.first, parameter.second);
	for (auto parameter : vector3Parameters) shader->setVec3(parameter.first, parameter.second);
}