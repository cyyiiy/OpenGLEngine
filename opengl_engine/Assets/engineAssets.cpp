#include "engineAssets.h"
#include <Utils/Color.h>
#include <Utils/filesystemUtils.h>

std::unordered_map<EngineAssets::VertexArrayID, std::shared_ptr<VertexArray>> EngineAssets::vertexArrays;
std::unordered_map<EngineAssets::ShaderID, std::shared_ptr<Shader>> EngineAssets::shaders;
std::unordered_map<EngineAssets::MaterialID, std::shared_ptr<Material>> EngineAssets::materials;
std::unordered_map<EngineAssets::TextureID, std::shared_ptr<Texture>> EngineAssets::textures;
std::unordered_map<EngineAssets::FontID, std::shared_ptr<Font>> EngineAssets::fonts;


std::shared_ptr<VertexArray> EngineAssets::GetVertexArray(VertexArrayID id)
{
	return vertexArrays.at(id);
}

std::shared_ptr<Shader> EngineAssets::GetShader(ShaderID id)
{
	return shaders.at(id);
}

std::shared_ptr<Material> EngineAssets::GetMaterial(MaterialID id)
{
	return materials.at(id);
}

std::shared_ptr<Texture> EngineAssets::GetTexture(TextureID id)
{
	return textures.at(id);
}

std::shared_ptr<Font> EngineAssets::GetFont(FontID id)
{
	return fonts.at(id);
}


void EngineAssets::LoadEngineAssets()
{
	// ------------------------- Vertex Arrays -------------------------

	vertexArrays.emplace(VertexArrayID::QuadHUD, std::make_shared<VertexArray>());
	vertexArrays.at(VertexArrayID::QuadHUD)->LoadVAQuadHUD();

	vertexArrays.emplace(VertexArrayID::Billboard, std::make_shared<VertexArray>());
	vertexArrays.at(VertexArrayID::Billboard)->LoadVABillboard();

	vertexArrays.emplace(VertexArrayID::Line, std::make_shared<VertexArray>());
	vertexArrays.at(VertexArrayID::Line)->LoadVALine();

	std::vector<Vertex> cube_vertices
	{
		// Positions                           // Normals                      // Tex coords
		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{ 0.5f, -0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{ 0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{ 0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{-0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{ 0.0f,  0.0f, -1.0f},   Vector2{0.0f, 0.0f}},

		Vertex{Vector3{-0.5f, -0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{ 0.5f, -0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{-0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{-0.5f, -0.5f,  0.5f},   Vector3{ 0.0f,  0.0f,  1.0f},   Vector2{0.0f, 0.0f}},

		Vertex{Vector3{-0.5f,  0.5f,  0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{-0.5f,  0.5f, -0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{-0.5f, -0.5f,  0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{-0.5f,  0.5f,  0.5f},   Vector3{-1.0f,  0.0f,  0.0f},   Vector2{1.0f, 0.0f}},

		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{ 0.5f,  0.5f, -0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{ 0.5f, -0.5f, -0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{ 0.5f, -0.5f, -0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{ 0.5f, -0.5f,  0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 1.0f,  0.0f,  0.0f},   Vector2{1.0f, 0.0f}},

		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{ 0.5f, -0.5f, -0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{ 0.5f, -0.5f,  0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{ 0.5f, -0.5f,  0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{-0.5f, -0.5f,  0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{-0.5f, -0.5f, -0.5f},   Vector3{ 0.0f, -1.0f,  0.0f},   Vector2{0.0f, 1.0f}},

		Vertex{Vector3{-0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{0.0f, 1.0f}},
		Vertex{Vector3{ 0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{1.0f, 1.0f}},
		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{ 0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{1.0f, 0.0f}},
		Vertex{Vector3{-0.5f,  0.5f,  0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{0.0f, 0.0f}},
		Vertex{Vector3{-0.5f,  0.5f, -0.5f},   Vector3{ 0.0f,  1.0f,  0.0f},   Vector2{0.0f, 1.0f}}
	};
	vertexArrays.emplace(VertexArrayID::Cube, std::make_shared<VertexArray>());
	vertexArrays.at(VertexArrayID::Cube)->LoadVAMesh(cube_vertices, {});


	// ------------------------- Shaders -------------------------------

	const Shader::LoadParams draw_debug_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Unlit/draw_debug.vert", "Shaders/Unlit/draw_debug.frag" }),
		ShaderType::Unlit
	};
	shaders.emplace(ShaderID::DrawDebug, Shader::Create(draw_debug_params));

	const Shader::LoadParams text_render_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Unlit/text_render.vert", "Shaders/Unlit/text_render.frag" }),
		ShaderType::Unlit
	};
	shaders.emplace(ShaderID::TextRender, Shader::Create(text_render_params));

	const Shader::LoadParams sprite_render_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Unlit/sprite_render.vert", "Shaders/Unlit/sprite_render.frag" }),
		ShaderType::Unlit
	};
	shaders.emplace(ShaderID::SpriteRender, Shader::Create(sprite_render_params));

	const Shader::LoadParams billboard_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Unlit/billboard.vert", "Shaders/Unlit/billboard.frag", "Shaders/Unlit/billboard.geom" }),
		ShaderType::Unlit
	};
	shaders.emplace(ShaderID::BillboardRender, Shader::Create(billboard_params)); 


	// ------------------------- Materials -----------------------------

	Material::LoadParams debug_mat_params(GetShader(ShaderID::DrawDebug));
	debug_mat_params.vec3Parameters.emplace("color", Color::green);
	materials.emplace(MaterialID::DrawDebug, std::make_shared<Material>(debug_mat_params));


	// ------------------------- Textures ------------------------------

	const Texture::LoadParams point_light_params = {
		"Default/Icons/icon_point_light.png",
		false
	};
	textures.emplace(TextureID::IconPointLight, Texture::Create(point_light_params));

	const Texture::LoadParams spot_light_params = {
		"Default/Icons/icon_spot_light.png",
		false
	};
	textures.emplace(TextureID::IconSpotLight, Texture::Create(spot_light_params));


	// ------------------------- Fonts ---------------------------------

	const Font::LoadParams arial_24_params = {
		"arial_font/arial.ttf",
		24,
		CharacterLoading::ASCII_128
	};
	fonts.emplace(FontID::Arial24, Font::Create(arial_24_params));
}

void EngineAssets::UnloadEngineAssets()
{
	vertexArrays.clear();
	shaders.clear();
	materials.clear();
	textures.clear();
	fonts.clear();
}