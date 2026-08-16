#include "defaultAssets.h"
#include "assetManager.h"
#include <ServiceLocator/locator.h>
#include <Utils/color.h>


void DefaultAssets::LoadDefaultAssets()
{
	Renderer& renderer = Locator::getRenderer();

	// Default textures (black)
	AssetManager::LoadTexture("default_black", "Default/black.png", false);


	// Default meshes (cube & plane)
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
	LoadMeshData cube_mesh_data{ cube_vertices, {}, 0 };
	AssetManager::CreateModelFromRawData("default_cube", { cube_mesh_data }, nullptr);

	std::vector<Vertex> plane_vertices
	{
		// Positions                         // Normals                   // Tex coords
		Vertex{Vector3{-0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f, -0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f, -0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f,  0.5f}},
		Vertex{Vector3{ 0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{ 0.5f,  0.5f}},
		Vertex{Vector3{-0.5f, 0.0f,  0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f,  0.5f}},
		Vertex{Vector3{-0.5f, 0.0f, -0.5f},  Vector3{ 0.0f, 1.0f, 0.0f},  Vector2{-0.5f, -0.5f}}
	};
	LoadMeshData plane_mesh_data{ plane_vertices, {}, 0 };
	AssetManager::CreateModelFromRawData("default_plane", { plane_mesh_data }, nullptr);


	// Default shaders (flat emissive & object lit)
	AssetManager::CreateShaderProgram("flat_emissive", ShaderType::Unlit, { "Shaders/Unlit/flat_emissive.vert", "Shaders/Unlit/flat_emissive.frag" });
	AssetManager::CreateShaderProgram("lit_object", ShaderType::Lit, { "Shaders/Lit/object_lit.vert", "Shaders/Lit/object_lit.frag" });

	// Default materials (black & white emissive)
	Material& black_emissive_mat = AssetManager::CreateMaterial("default_black_emissive", AssetManager::GetShader("flat_emissive"));
	black_emissive_mat.addParameter("emissive", Color::black);
	renderer.AddMaterial(&black_emissive_mat);

	Material& white_emissive_mat = AssetManager::CreateMaterial("default_white_emissive", AssetManager::GetShader("flat_emissive"));
	white_emissive_mat.addParameter("emissive", Color::white);
	renderer.AddMaterial(&white_emissive_mat);


	// Default audio collision
	AssetManager::RegisterAudioCollisionType("default_audio_collision", AudioCollisionOcclusion{ 1.0f, 0.5f });
}