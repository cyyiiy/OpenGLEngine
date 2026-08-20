#include "shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <limits>

#include <Maths/vector2.h>
#include <Maths/vector3.h>
#include <Maths/vector4.h>

#include <ServiceLocator/locator.h>


Shader::Shader(unsigned int _ID, ShaderType _type) :
	IAsset(), ID(_ID), type(_type)
{}

Shader::~Shader()
{
	glDeleteProgram(ID);
}


std::string Shader::GetTypeName()
{
	return "Shader";
}

std::shared_ptr<Shader> Shader::Create(const LoadParams& params)
{
	unsigned int id;
	std::unordered_map<GLenum, std::string> shader_parts = ConvertShaderPaths(params.shaderPaths);

	// Check for the mandatory shader parts
	if (shader_parts.find(GL_VERTEX_SHADER) == shader_parts.end())
	{
		Locator::getLog().LogMessage_Category("Shader: Cannot load a shader without a vertex shader part!", LogCategory::Error);
		return nullptr;
	}
	if (shader_parts.find(GL_FRAGMENT_SHADER) == shader_parts.end())
	{
		Locator::getLog().LogMessage_Category("Shader: Cannot load a shader without a fragment shader part!", LogCategory::Error);
		return nullptr;
	}

	// Prepare the shader program
	id = glCreateProgram();
	std::vector<unsigned int> shader_parts_ids;

	// Load the individual shader parts
	for (auto& shader_part : shader_parts)
	{
		// Load the individual shader part
		unsigned int shader_part_id;
		const bool success = LoadShaderPart(shader_part.first, shader_part.second, shader_part_id);

		// Handle failed loads
		if (!success)
		{
			switch (shader_part.first)
			{
			case GL_VERTEX_SHADER:
				Locator::getLog().LogMessage_Category("Shader: Cannot load a shader with an invalid vertex shader part!", LogCategory::Error);
				glDeleteProgram(id);
				return nullptr;

			case GL_FRAGMENT_SHADER:
				Locator::getLog().LogMessage_Category("Shader: Cannot load a shader with an invalid fragment shader part!", LogCategory::Error);
				glDeleteProgram(id);
				return nullptr;

			default:
				Locator::getLog().LogMessage_Category("Shader: Failed to load a non-mandatory shader part. The shader will work, but will be incomplete.", LogCategory::Warning);
				continue;
			}
		}

		// Add the shader part id to the vector
		shader_parts_ids.push_back(shader_part_id);
	}

	// Attach the shader parts to the shader program and link the program
	for (unsigned int& shader_part_id : shader_parts_ids)
	{
		glAttachShader(id, shader_part_id);
	}
	glLinkProgram(id);

	// Delete the shader parts once they are link into the program
	for (unsigned int& shader_part_id : shader_parts_ids)
	{
		glDeleteShader(shader_part_id);
	}

	// Check if the shader program linked correctly
	int success;
	char info_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, info_log);
		Locator::getLog().LogMessage_Category("Shader: Failed to link the shader program. | " + std::string(info_log), LogCategory::Error);
		glDeleteProgram(id);
		return nullptr;
	}

	return std::make_shared<Shader>(id, params.shaderType);
}

Shader::LoadParams Shader::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}

uint64_t Shader::getAssetMemorySize() const
{
	return sizeof(Shader);
}

uint64_t Shader::getAssetGpuSize() const
{
	// Note: the size of a compiled shader is so small compared to textures for example that it is basically a useless information
	// Also there is no reliable way to know the size of a compiled shader as it vary depending on the GPU and the driver
	return 0;
}


std::unordered_map<GLenum, std::string> Shader::ConvertShaderPaths(std::vector<std::filesystem::path> shaderPaths)
{
	// Initialize the return map
	std::unordered_map<GLenum, std::string> shader_parts;

	// Check the shader path extension and store them into the map with the correct key
	for (std::filesystem::path& shader_path_converted : shaderPaths)
	{
		const std::string path_extension = shader_path_converted.extension().string();
		if (path_extension == ".vert" || path_extension == ".vs")
		{
			if (shader_parts.find(GL_VERTEX_SHADER) != shader_parts.end())
			{
				Locator::getLog().LogMessage_Category("Shader: Cannot load more than one vertex shader to a single shader program!", LogCategory::Error);
				continue;
			}
			shader_parts.emplace(GL_VERTEX_SHADER, shader_path_converted.string());
			continue;
		}
		if (path_extension == ".frag" || path_extension == ".fs")
		{
			if (shader_parts.find(GL_FRAGMENT_SHADER) != shader_parts.end())
			{
				Locator::getLog().LogMessage_Category("Shader: Cannot load more than one fragment shader to a single shader program!", LogCategory::Error);
				continue;
			}
			shader_parts.emplace(GL_FRAGMENT_SHADER, shader_path_converted.string());
			continue;
		}
		if (path_extension == ".geom" || path_extension == ".gs")
		{
			if (shader_parts.find(GL_GEOMETRY_SHADER) != shader_parts.end())
			{
				Locator::getLog().LogMessage_Category("Shader: Cannot load more than one geometry shader to a single shader program!", LogCategory::Error);
				continue;
			}
			shader_parts.emplace(GL_GEOMETRY_SHADER, shader_path_converted.string());
			continue;
		}
		// Unknown shader extension
		Locator::getLog().LogMessage_Category("Shader: Unknown shader extension: \"" + path_extension + "\"", LogCategory::Error);
	}

	return shader_parts;
}

bool Shader::LoadShaderPart(const GLenum shaderPartType, const std::string& shaderPartPath, unsigned int& outShaderPartId)
{
	//  Step 1: Load the shader part source code from path
	//  ==================================================

	// Initialize shader loading variables
	std::ifstream shader_part_file;
	std::string shader_part_code;

	// Ensure the ifstream object can throw exceptions
	shader_part_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read the shader code
	try
	{
		// Open the shader file
		shader_part_file.open(shaderPartPath);

		// Read the shader file content into stream
		std::stringstream shader_part_stream;
		shader_part_stream << shader_part_file.rdbuf();

		// Close the shader file
		shader_part_file.close();

		// Convert the stream into a string
		shader_part_code = shader_part_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		Locator::getLog().LogMessage_Category("Shader: Failed to read shader part '" + shaderPartPath + "'.", LogCategory::Error);
		return false;
	}

	// Convert the shader code into a char array
	const char* shader_part_code_char = shader_part_code.c_str();


	//  Step 2: Compile the shader part
	//  ===============================

	// Compile the shader part
	outShaderPartId = glCreateShader(shaderPartType);
	glShaderSource(outShaderPartId, 1, &shader_part_code_char, NULL);
	glCompileShader(outShaderPartId);

	// Check if the shader part compiled correctly
	int success;
	char info_log[512];
	glGetShaderiv(outShaderPartId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(outShaderPartId, 512, NULL, info_log);
		Locator::getLog().LogMessage_Category("Shader: Failed to compile shader part. | " + std::string(info_log), LogCategory::Error);
		return false;
	}

	return true;
}


void Shader::use() const
{
	glUseProgram(ID);
}


void Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const Vector2& value) const
{
	setVec2(name, value.x, value.y);
}

void Shader::setVec2(const std::string& name, const float xValue, const float yValue) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), xValue, yValue);
}

void Shader::setVec3(const std::string& name, const Vector3& value) const
{
	setVec3(name, value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& name, const float xValue, const float yValue, const float zValue) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), xValue, yValue, zValue);
}

void Shader::setVec4(const std::string& name, const Vector4& value) const
{
	setVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::setVec4(const std::string& name, const float xValue, const float yValue, const float zValue, const float wValue) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), xValue, yValue, zValue, wValue);
}

void Shader::setBoolArray(const std::string& name, const bool* firstValue, const int arraySize) const
{
	glUniform1iv(glGetUniformLocation(ID, name.c_str()), arraySize, (int*)firstValue);
}

void Shader::setIntArray(const std::string& name, const int* firstValue, const int arraySize) const
{
	glUniform1iv(glGetUniformLocation(ID, name.c_str()), arraySize, firstValue);
}

void Shader::setFloatArray(const std::string& name, const float* firstValue, const int arraySize) const
{
	glUniform1fv(glGetUniformLocation(ID, name.c_str()), arraySize, firstValue);
}

void Shader::setVec2Array(const std::string& name, const Vector2* firstValue, const int arraySize) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), arraySize, firstValue->getAsFloatPtr());
}

void Shader::setVec3Array(const std::string& name, const Vector3* firstValue, const int arraySize) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), arraySize, firstValue->getAsFloatPtr());
}

void Shader::setVec4Array(const std::string& name, const Vector4* firstValue, const int arraySize) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), arraySize, firstValue->getAsFloatPtr());
}

void Shader::setMatrix4(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, value);
}

void Shader::setMatrix4Array(const std::string& name, const float* firstValue, const int arraySize) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), arraySize, GL_TRUE, firstValue);
}
