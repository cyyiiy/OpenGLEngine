#include "defaultAssets.h"
#include "assetManager.h"
#include <Rendering/texture.h>
#include <Rendering/Model/model.h>
#include <Rendering/shader.h>
#include <Rendering/material.h>
#include <Rendering/Text/font.h>
#include <ServiceLocator/locator.h>
#include <Utils/filesystemUtils.h>
#include <Utils/color.h>


void DefaultAssets::LoadDefaultAssets()
{
	// Default textures (black)
	const Texture::LoadParams texture_black_params = { "Default/black.png", false };
	AssetManager::LoadAsset<Texture>("default_black", texture_black_params);


	// Default meshes (cube & plane)
	const std::vector<Vertex> cube_vertices
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
	const MeshVerticesData cube_mesh_data = { cube_vertices, {}, 0 };
	const Model::RawVerticesParams model_cube_params = { cube_mesh_data, nullptr };
	AssetManager::LoadAsset<Model>("default_cube", model_cube_params);

	const std::vector<Vertex> plane_vertices
	{
		// Positions                         // Normals                   // Tex coords
		Vertex{Vector3{-0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f, -0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f, -0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f,  0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f,  0.5f}},
		Vertex{Vector3{-0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f,  0.5f}},
		Vertex{Vector3{-0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f, -0.5f}}
	};
	const MeshVerticesData plane_mesh_data{ plane_vertices, {}, 0 };
	const Model::RawVerticesParams model_plane_params = { plane_mesh_data, nullptr };
	AssetManager::LoadAsset<Model>("default_plane", model_plane_params);


	// Default shaders (flat emissive, object lit & flat lit)
	const Shader::LoadParams shader_flat_emissive_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Unlit/flat_emissive.vert", "Shaders/Unlit/flat_emissive.frag" }),
		ShaderType::Unlit
	};
	AssetManager::LoadAsset<Shader>("flat_emissive", shader_flat_emissive_params);

	const Shader::LoadParams shader_lit_object_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Lit/object_lit.vert", "Shaders/Lit/object_lit.frag" }),
		ShaderType::Lit
	};
	AssetManager::LoadAsset<Shader>("lit_object", shader_lit_object_params);

	const Shader::LoadParams shader_lit_flat_color_params = {
		FilesystemUtils::ConvertStringsToPaths({ "Shaders/Lit/flat_lit.vert", "Shaders/Lit/flat_lit.frag" }),
		ShaderType::Lit
	};
	AssetManager::LoadAsset<Shader>("lit_flat_color", shader_lit_flat_color_params);

	// Default materials (black & white emissive, grey lit)
	Material::LoadParams material_black_emissive_params(AssetManager::GetAsset<Shader>("flat_emissive"));
	material_black_emissive_params.vec3Parameters.emplace("emissive", Color::black);
	AssetManager::LoadAsset<Material>("default_black_emissive", material_black_emissive_params);

	Material::LoadParams material_white_emissive_params(AssetManager::GetAsset<Shader>("flat_emissive"));
	material_white_emissive_params.vec3Parameters.emplace("emissive", Color::white);
	AssetManager::LoadAsset<Material>("default_white_emissive", material_white_emissive_params);

	Material::LoadParams material_grey_lit_params(AssetManager::GetAsset<Shader>("lit_flat_color"));
	material_grey_lit_params.vec3Parameters.emplace("flatColor", Color::grey);
	material_grey_lit_params.floatParameters.emplace("specularStrength", 0.2f);
	AssetManager::LoadAsset<Material>("default_grey_lit", material_grey_lit_params);

	// Default fonts (arial 64)
	Font::LoadParams font_arial_64_params = { "arial_font/arial.ttf", 64, CharacterLoading::ASCII_128 };
	AssetManager::LoadAsset<Font>("arial_64", font_arial_64_params);
}