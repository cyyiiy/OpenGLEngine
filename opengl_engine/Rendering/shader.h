#pragma once
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned int GLenum;

enum class ShaderType : uint8_t
{
	Null,
	Lit,
	Unlit
};


class Shader : public IAsset
{
public:
	// Asset part
	struct LoadParams
	{
		std::vector<std::filesystem::path> shaderPaths;
		ShaderType shaderType;
	};

	Shader(unsigned int _ID, ShaderType _type);
	~Shader();

	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<Shader> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// Shader part
	void use() const;
	ShaderType getShaderType() const { return type; }

	// Shader uniform setters
	void setBool(const std::string& name, const bool value) const;
	void setInt(const std::string& name, const int value) const;
	void setFloat(const std::string& name, const float value) const;
	void setVec2(const std::string& name, const struct Vector2& value) const;
	void setVec2(const std::string& name, const float xValue, const float yValue) const;
	void setVec3(const std::string& name, const struct Vector3& value) const;
	void setVec3(const std::string& name, const float xValue, const float yValue, const float zValue) const;
	void setVec4(const std::string& name, const struct Vector4& value) const;
	void setVec4(const std::string& name, const float xValue, const float yValue, const float zValue, const float wValue) const;

	void setBoolArray(const std::string& name, const bool* firstValue, const int arraySize) const;
	void setIntArray(const std::string& name, const int* firstValue, const int arraySize) const;
	void setFloatArray(const std::string& name, const float* firstValue, const int arraySize) const;
	void setVec2Array(const std::string& name, const struct Vector2* firstValue, const int arraySize) const;
	void setVec3Array(const std::string& name, const struct Vector3* firstValue, const int arraySize) const;
	void setVec4Array(const std::string& name, const struct Vector4* firstValue, const int arraySize) const;

	void setMatrix4(const std::string& name, const float* value) const;
	void setMatrix4Array(const std::string& name, const float* firstValue, const int arraySize) const;

private:
	unsigned int ID{ 0 };
	ShaderType type{ ShaderType::Null };

	static std::unordered_map<GLenum, std::string> ConvertShaderPaths(std::vector<std::filesystem::path> shaderPaths);
	static bool LoadShaderPart(const GLenum shaderPartType, const std::string& shaderPartPath, unsigned int& outShaderPartId);
};