#pragma once
#include <Rendering/Model/vertexArray.h>
#include <Rendering/shader.h>
#include <Rendering/material.h>
#include <Rendering/texture.h>
#include <Rendering/Text/font.h>
#include <unordered_map>
#include <memory>


class EngineAssets
{
public:
	enum class VertexArrayID : uint8_t
	{
		QuadHUD,
		Billboard,
		Line,
		Cube,
	};

	enum class ShaderID : uint8_t
	{
		DrawDebug,
		TextRender,
		SpriteRender,
		BillboardRender,
	};

	enum class MaterialID : uint8_t
	{
		DrawDebug,
	};

	enum class TextureID : uint8_t
	{
		IconPointLight,
		IconSpotLight,
	};

	enum class FontID : uint8_t
	{
		Arial24,
		Arial64,
	};


	[[nodiscard]] static const VertexArray& GetVertexArray(VertexArrayID id);
	[[nodiscard]] static const Shader& GetShader(ShaderID id);
	[[nodiscard]] static const Material& GetMaterial(MaterialID id);
	[[nodiscard]] static const Texture& GetTexture(TextureID id);
	[[nodiscard]] static const Font& GetFont(FontID id);


private:
	friend class Engine;

	static void LoadEngineAssets();
	static void UnloadEngineAssets();

	static std::unordered_map<VertexArrayID, std::unique_ptr<VertexArray>> vertexArrays;
	static std::unordered_map<ShaderID, std::unique_ptr<Shader>> shaders;
	static std::unordered_map<MaterialID, std::unique_ptr<Material>> materials;
	static std::unordered_map<TextureID, std::unique_ptr<Texture>> textures;
	static std::unordered_map<FontID, std::unique_ptr<Font>> fonts;
};

