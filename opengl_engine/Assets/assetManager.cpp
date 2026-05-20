#include "assetManager.h"
#include "ServiceLocator/locator.h"



// --------------------------------------------------------------
//            Assets lists
// --------------------------------------------------------------

std::unordered_map<std::string, std::unique_ptr<Texture>> AssetManager::textures;
std::unordered_map<std::string, std::unique_ptr<VertexArray>> AssetManager::vertexArrays;
std::unordered_map<std::string, std::unique_ptr<Mesh>> AssetManager::meshesSingle;
std::unordered_map<std::string, std::unique_ptr<MeshCollection>> AssetManager::meshesCollection;
std::unordered_map<std::string, std::unique_ptr<Model>> AssetManager::models;
std::unordered_map<std::string, std::unique_ptr<Shader>> AssetManager::shaders;
std::unordered_map<std::string, std::unique_ptr<Material>> AssetManager::materials;
std::unordered_map<std::string, std::unique_ptr<MaterialCollection>> AssetManager::materialsCollection;
std::unordered_map<std::string, std::unique_ptr<Font>> AssetManager::fonts;
std::unordered_map<std::string, std::unique_ptr<AudioSound>> AssetManager::sounds;
std::unordered_map<std::string, AudioCollisionOcclusion> AssetManager::audioCollisionTypes;



// --------------------------------------------------------------
//            Textures
// --------------------------------------------------------------

void AssetManager::LoadTexture(const std::string& name, const std::string& texturePath, const bool flipVertical)
{
	if (textures.find(name) != textures.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to load a texture with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	textures.emplace(name, std::make_unique<Texture>(texturePath, flipVertical));
}

Texture& AssetManager::GetTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a texture with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *textures["null_texture"];
	}

	return *textures[name];
}

void AssetManager::DeleteTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a texture with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	textures.erase(name);
}



// --------------------------------------------------------------
//            Vertex Arrays
// --------------------------------------------------------------

VertexArray& AssetManager::CreateVertexArray(const std::string& name)
{
	if (vertexArrays.find(name) != vertexArrays.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a vertex array with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *vertexArrays["null_vertexarray"];
	}

	vertexArrays.emplace(name, std::make_unique<VertexArray>());
	return *vertexArrays[name];
}

VertexArray& AssetManager::GetVertexArray(const std::string& name)
{
	if (vertexArrays.find(name) == vertexArrays.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a vertex array with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *vertexArrays["null_vertexarray"];
	}

	return *vertexArrays[name];
}

void AssetManager::DeleteVertexArray(const std::string& name)
{
	if (vertexArrays.find(name) == vertexArrays.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a vertex array with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	vertexArrays.erase(name);
}



// --------------------------------------------------------------
//            Meshes
// --------------------------------------------------------------

void AssetManager::LoadSingleMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	if (meshesSingle.find(name) != meshesSingle.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to load a single mesh with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	meshesSingle.emplace(name, std::make_unique<Mesh>(vertices, indices));
}

void AssetManager::LoadSingleMesh(const std::string& name, const std::string& filepath)
{
	if (meshesSingle.find(name) != meshesSingle.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to load a single mesh with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	LoadMeshData mesh_data = AssetMesh::LoadSingleMesh(filepath);
	meshesSingle.emplace(name, std::make_unique<Mesh>(mesh_data.vertices, mesh_data.indices, mesh_data.matId));
}

void AssetManager::LoadMeshCollection(const std::string& name, const std::string& filepath)
{
	if (meshesCollection.find(name) != meshesCollection.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to load a mesh collection with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	meshesCollection.emplace(name, std::make_unique<MeshCollection>());
	MeshCollection& mesh_collection = *meshesCollection[name];

	std::vector<LoadMeshData> meshes_datas = AssetMesh::LoadMeshCollection(filepath);
	mesh_collection.collection.reserve(meshes_datas.size());
	for (auto& mesh_data : meshes_datas)
	{
		mesh_collection.collection.push_back(std::make_unique<Mesh>(mesh_data.vertices, mesh_data.indices, mesh_data.matId));
	}
}

Mesh& AssetManager::GetSingleMesh(const std::string& name)
{
	if (meshesSingle.find(name) == meshesSingle.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a single mesh with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *meshesSingle["null_mesh"];
	}

	return *meshesSingle[name];
}

MeshCollection& AssetManager::GetMeshCollection(const std::string& name)
{
	if (meshesCollection.find(name) == meshesCollection.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a mesh collection with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *meshesCollection["null_collection"];
	}

	return *meshesCollection[name];
}

void AssetManager::DeleteSingleMesh(const std::string& name)
{
	if (meshesSingle.find(name) == meshesSingle.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a single mesh with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	meshesSingle.erase(name);
}

void AssetManager::DeleteMeshCollection(const std::string& name)
{
	if (meshesCollection.find(name) == meshesCollection.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a mesh collection with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	meshesCollection.erase(name);
}



// --------------------------------------------------------------
//            Models
// --------------------------------------------------------------

Model& AssetManager::CreateModel(const std::string& name)
{
	if (models.find(name) != models.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a model with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *models["null_model"];
	}

	models.emplace(name, std::make_unique<Model>());
	return *models[name];
}

Model& AssetManager::GetModel(const std::string& name)
{
	if (models.find(name) == models.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a model with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *models["null_model"];
	}

	return *models[name];
}

void AssetManager::DeleteModel(const std::string& name)
{
	if (models.find(name) == models.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a model with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	models.erase(name);
}



// --------------------------------------------------------------
//            Shaders
// --------------------------------------------------------------

void AssetManager::CreateShaderProgram(const std::string& name, const std::string& vertexName, const std::string& fragmentName, const ShaderType shaderType)
{
	if (shaders.find(name) != shaders.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a shader with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	shaders.emplace(name, std::make_unique<Shader>(vertexName, fragmentName, shaderType));
}

Shader& AssetManager::GetShader(const std::string& name)
{
	if (shaders.find(name) == shaders.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a shader with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *shaders["null_shader"];
	}

	return *shaders[name];
}

void AssetManager::DeleteShader(const std::string& name)
{
	if (shaders.find(name) == shaders.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a shader with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	shaders.erase(name);
}



// --------------------------------------------------------------
//            Materials
// --------------------------------------------------------------

Material& AssetManager::CreateMaterial(const std::string& name, Shader& shaderUsed)
{
	if (materials.find(name) != materials.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a material with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *materials["null_material"];
	}

	materials.emplace(name, std::make_unique<Material>(shaderUsed));
	Locator::getRenderer().AddMaterial(materials[name].get());
	return *materials[name];
}

MaterialCollection& AssetManager::CreateMaterialCollection(const std::string& name, const std::vector<Material*>& materialCollection)
{
	if (materialsCollection.find(name) != materialsCollection.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a material collection with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *materialsCollection["null_mat_collection"];
	}

	materialsCollection.emplace(name, std::make_unique<MaterialCollection>(materialCollection));
	return *materialsCollection[name];
}

Material& AssetManager::GetMaterial(const std::string& name)
{
	if (materials.find(name) == materials.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a material with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *materials["null_material"];
	}

	return *materials[name];
}

MaterialCollection& AssetManager::GetMaterialCollection(const std::string& name)
{
	if (materialsCollection.find(name) == materialsCollection.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a material collection with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *materialsCollection["null_mat_collection"];
	}

	return *materialsCollection[name];
}

void AssetManager::DeleteMaterial(const std::string& name)
{
	if (materials.find(name) == materials.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a material with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	Locator::getRenderer().RemoveMaterial(materials[name].get());
	materials.erase(name);
}



// --------------------------------------------------------------
//            Fonts
// --------------------------------------------------------------

void AssetManager::LoadFont(const std::string& name, const std::string& fontPath, const int size, const CharacterLoading charLoadSetting)
{
	if (fonts.find(name) != fonts.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a font with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	fonts.emplace(name, std::make_unique<Font>(fontPath, size, charLoadSetting));
}

Font& AssetManager::GetFont(const std::string& name)
{
	if (fonts.find(name) == fonts.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a font with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *fonts["null_font"];
	}

	return *fonts[name];
}

void AssetManager::DeleteFont(const std::string& name)
{
	if (fonts.find(name) == fonts.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a font with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	fonts.erase(name);
}



// --------------------------------------------------------------
//            Sounds
// --------------------------------------------------------------

AudioSound& AssetManager::CreateSound(const std::string& name, const std::string& filePath, const SoundSettings settings)
{
	if (sounds.find(name) != sounds.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a sound with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *sounds["null_sound"];
	}

	Audio& audio_manager = Locator::getAudio();
	sounds.emplace(name, std::make_unique<AudioSound>(audio_manager.LoadSound(filePath, settings).sound, settings));
	return *sounds[name];
}

AudioSound& AssetManager::GetSound(const std::string& name)
{
	if (sounds.find(name) == sounds.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a sound with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *sounds["null_sound"];
	}

	return *sounds[name];
}

void AssetManager::DeleteSound(const std::string& name)
{
	if (sounds.find(name) == sounds.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete a sound with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	sounds.erase(name);
}



// --------------------------------------------------------------
//            Audio Collision Type
// --------------------------------------------------------------

AudioCollisionOcclusion& AssetManager::RegisterAudioCollisionType(const std::string& name, AudioCollisionOcclusion audioCollisionType)
{
	if (audioCollisionTypes.find(name) != audioCollisionTypes.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create an audioCollisionType with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return audioCollisionTypes["null_audio_collision_type"];
	}

	audioCollisionTypes.emplace(name, audioCollisionType);
	return audioCollisionTypes[name];
}

AudioCollisionOcclusion& AssetManager::GetAudioCollisionType(const std::string& name)
{
	if (audioCollisionTypes.find(name) == audioCollisionTypes.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get an audioCollisionType with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return audioCollisionTypes["null_audio_collision_type"];
	}

	return audioCollisionTypes[name];
}

void AssetManager::DeleteAudioCollisionType(const std::string& name)
{
	if (audioCollisionTypes.find(name) == audioCollisionTypes.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to delete an audioCollisionType with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	audioCollisionTypes.erase(name);
}




// --------------------------------------------------------------
//           Null Assets
// --------------------------------------------------------------

void AssetManager::LoadNullAssets()
{
	LoadTexture("null_texture", "Default/notexture.png", false);
	vertexArrays.emplace("null_vertexarray", std::make_unique<VertexArray>());
	meshesSingle.emplace("null_mesh", std::make_unique<Mesh>());
	meshesCollection.emplace("null_collection", std::make_unique<MeshCollection>());
	models.emplace("null_model", std::make_unique<Model>());
	shaders.emplace("null_shader", std::make_unique<Shader>());
	materials.emplace("null_material", std::make_unique<Material>(GetShader("null_shader")));
	materialsCollection.emplace("null_mat_collection", std::make_unique<MaterialCollection>());
	fonts.emplace("null_font", std::make_unique<Font>());
	sounds.emplace("null_sound", std::make_unique<AudioSound>(nullptr, 0));
	audioCollisionTypes.emplace("null_audio_collision_type", AudioCollisionOcclusion{ 0.0f, 0.0f });
}
