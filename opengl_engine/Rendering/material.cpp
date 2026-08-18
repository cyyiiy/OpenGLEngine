#include "material.h"
#include <glad/glad.h>
#include <Assets/assetsIDs.h>
#include <string>


Material::Material(Shader& shaderUsed) : shader(shaderUsed)
{
	uniqueID = AssetsIDs::GenerateMaterialUniqueID();
}

Material::~Material()
{
}

void Material::use()
{
	// TODO: Check if the shader is not nullptr?

	//  assume the shader is already in use (the rendering process should have done it)

	unsigned int tex_activated = 0;
	for (auto tex_type : textures)
	{
		TextureType type = tex_type.first;
		unsigned int number = 0;

		for (auto texture : tex_type.second)
		{
			glActiveTexture(GL_TEXTURE0 + tex_activated); //  activate texture unit first
			std::string str_number = std::to_string(++number);

			shader.setInt("material." + TypeToString(type) + str_number, tex_activated); //  then set the sampler to the correct texture unit
			texture->use(); //  finally bind the texture

			tex_activated++;
		}
	}

	glActiveTexture(GL_TEXTURE0); //  reinitialisate the texture activation

	for (auto parameter : boolParameters) shader.setBool(parameter.first, parameter.second);
	for (auto parameter : intParameters) shader.setInt(parameter.first, parameter.second);
	for (auto parameter : floatParameters) shader.setFloat(parameter.first, parameter.second);
	for (auto parameter : vector3Parameters) shader.setVec3(parameter.first, parameter.second);
}


void Material::addTexture(Texture* texture, TextureType type)
{
	textures[type].push_back(texture);
}

void Material::addParameter(std::string name, bool boolParameter)
{
	boolParameters[name] = boolParameter;
}

void Material::addParameter(std::string name, int intParameter)
{
	intParameters[name] = intParameter;
}

void Material::addParameter(std::string name, float floatParameter)
{
	floatParameters[name] = floatParameter;
}

void Material::addParameter(std::string name, Vector3 vec3Parameter)
{
	vector3Parameters[name] = vec3Parameter;
}

void Material::addParameter(std::string name, float vec3ParameterX, float vec3ParameterY, float vec3ParameterZ)
{
	vector3Parameters[name] = Vector3{ vec3ParameterX, vec3ParameterY, vec3ParameterZ };
}







std::string Material::TypeToString(TextureType textureType)
{
	switch (textureType)
	{

	case TextureType::Undefined:
		return std::string("");

	case TextureType::Diffuse:
		return std::string("texture_diffuse");

	case TextureType::Specular:
		return std::string("texture_specular");

	case TextureType::Emissive:
		return std::string("texture_emissive");

	default:
		return std::string("");

	}
}

bool Material::operator==(const Material& other) const
{
	return uniqueID == other.uniqueID;
}

bool Material::operator!=(const Material& other) const
{
	return uniqueID != other.uniqueID;
}