#include "engineAssets.h"
#include <Utils/Color.h>

std::unordered_map<EngineAssets::VertexArrayID, std::shared_ptr<VertexArray>> EngineAssets::vertexArrays;
std::unordered_map<EngineAssets::ShaderID, std::shared_ptr<Shader>> EngineAssets::shaders;
std::unordered_map<EngineAssets::MaterialID, std::shared_ptr<Material>> EngineAssets::materials;
std::unordered_map<EngineAssets::TextureID, std::shared_ptr<Texture>> EngineAssets::textures;
std::unordered_map<EngineAssets::FontID, std::shared_ptr<Font>> EngineAssets::fonts;


const VertexArray& EngineAssets::GetVertexArray(VertexArrayID id)
{
	return *vertexArrays.at(id);
}

Shader& EngineAssets::GetShader(ShaderID id)
{
	return *shaders.at(id);
}

Material& EngineAssets::GetMaterial(MaterialID id)
{
	return *materials.at(id);
}

const Texture& EngineAssets::GetTexture(TextureID id)
{
	return *textures.at(id);
}

Font& EngineAssets::GetFont(FontID id)
{
	return *fonts.at(id);
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

	const std::vector<std::string> draw_debug_paths = { "Shaders/Unlit/draw_debug.vert", "Shaders/Unlit/draw_debug.frag" };
	shaders.emplace(ShaderID::DrawDebug, std::make_shared<Shader>(draw_debug_paths, ShaderType::Unlit));

	const std::vector<std::string> text_render_paths = { "Shaders/Unlit/text_render.vert", "Shaders/Unlit/text_render.frag" };
	shaders.emplace(ShaderID::TextRender, std::make_shared<Shader>(text_render_paths, ShaderType::Unlit));

	const std::vector<std::string> sprite_render_paths = { "Shaders/Unlit/sprite_render.vert", "Shaders/Unlit/sprite_render.frag" };
	shaders.emplace(ShaderID::SpriteRender, std::make_shared<Shader>(sprite_render_paths, ShaderType::Unlit));

	const std::vector<std::string> billboard_paths = { "Shaders/Unlit/billboard.vert", "Shaders/Unlit/billboard.frag", "Shaders/Unlit/billboard.geom" };
	shaders.emplace(ShaderID::BillboardRender, std::make_shared<Shader>(billboard_paths, ShaderType::Unlit));


	// ------------------------- Materials -----------------------------

	materials.emplace(MaterialID::DrawDebug, std::make_shared<Material>(*shaders.at(ShaderID::DrawDebug)));
	materials.at(MaterialID::DrawDebug)->addParameter("color", Color::green);


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

	const std::string arial_24_path = "arial_font/arial.ttf";
	fonts.emplace(FontID::Arial24, std::make_shared<Font>(arial_24_path, 24, CharacterLoading::ASCII_128));

	const std::string arial_64_path = "arial_font/arial.ttf";
	fonts.emplace(FontID::Arial64, std::make_shared<Font>(arial_64_path, 64, CharacterLoading::ASCII_128));
}

void EngineAssets::UnloadEngineAssets()
{
	vertexArrays.clear();
	shaders.clear();
	materials.clear();
	textures.clear();
	fonts.clear();
}