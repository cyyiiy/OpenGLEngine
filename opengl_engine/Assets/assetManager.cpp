#include "assetManager.h"
#include "MeshModel/modelLoader.h"
#include <Utils/filesystemUtils.h>
#include <ServiceLocator/locator.h>



// --------------------------------------------------------------
//            Assets lists
// --------------------------------------------------------------

std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::textures;
std::unordered_map<std::string, std::unique_ptr<VertexArray>> AssetManager::vertexArrays;
std::unordered_map<std::string, std::unique_ptr<Model>> AssetManager::models;
std::unordered_map<std::string, std::shared_ptr<Shader>> AssetManager::shaders;
std::unordered_map<std::string, std::unique_ptr<Material>> AssetManager::materials;
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

	Texture::LoadParams texture_params = { texturePath, flipVertical };
	std::shared_ptr<Texture> texture = Texture::Create(texture_params);
	if (texture) textures.emplace(name, texture);
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
//            Models
// --------------------------------------------------------------

Model& AssetManager::LoadModel(const std::string& name, const std::string& modelPath, Material* fillMaterial)
{
	if (models.find(name) != models.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to load a model with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *models["null_model"];
	}

	models.emplace(name, std::make_unique<Model>(ModelLoader::LoadModel(modelPath, fillMaterial)));
	return *models[name];
}

Model& AssetManager::CreateModelFromRawData(const std::string& name, std::vector<LoadMeshData> rawData, Material* fillMaterial)
{
	if (models.find(name) != models.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a model with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return *models["null_model"];
	}

	models.emplace(name, std::make_unique<Model>(rawData, fillMaterial));
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

void AssetManager::CreateShaderProgram(const std::string& name, const ShaderType shaderType, std::vector<std::string> shaderPaths)
{
	if (shaders.find(name) != shaders.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to create a shader with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	Shader::LoadParams shader_params = { FilesystemUtils::ConvertStringsToPaths(shaderPaths), shaderType };
	std::shared_ptr<Shader> shader = Shader::Create(shader_params);
	if (shader) shaders.emplace(name, shader);
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

Material& AssetManager::GetMaterial(const std::string& name)
{
	if (materials.find(name) == materials.end())
	{
		Locator::getLog().LogMessage_Category("Asset Manager: Tried to get a material with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return *materials["null_material"];
	}

	return *materials[name];
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
	models.emplace("null_model", std::make_unique<Model>(std::vector<LoadMeshData>{}, nullptr));
	//shaders.emplace("null_shader", std::make_unique<Shader>());
	//materials.emplace("null_material", std::make_unique<Material>(GetShader("null_shader")));
	fonts.emplace("null_font", std::make_unique<Font>());
	sounds.emplace("null_sound", std::make_unique<AudioSound>(nullptr, 0));
	audioCollisionTypes.emplace("null_audio_collision_type", AudioCollisionOcclusion{ 0.0f, 0.0f });
}

void AssetManager::ClearAllAssets()
{
	textures.clear();
	vertexArrays.clear();
	models.clear();
	shaders.clear();
	materials.clear();
	fonts.clear();
	sounds.clear();
	audioCollisionTypes.clear();
}
