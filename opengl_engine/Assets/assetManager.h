#pragma once
#include "assetMesh.h"
#include <Rendering/texture.h>
#include <Rendering/shader.h>
#include <Rendering/material.h>
#include <Rendering/Model/model.h>
#include <Rendering/Text/font.h>
#include <Audio/audioSound.h>

#include <unordered_map>
#include <string>


class AssetManager
{
public:
	/**
	* Load the "null" assest (the assets that will be returned if user ask for a non-existing asset.
	*/
	static void LoadNullAssets();



// -----------------------------------------------------------------------------
//                 Textures
// -----------------------------------------------------------------------------

	/**
	* Load a texture from file and stores it.
	* @param	name			The name you want to give to this texture in the asset storage.
	* @param	texturePath		The path to the texture file to read.
	* @param	flipVertical	(optionnal) Flip the texture vertically.
	*/
	static void LoadTexture(const std::string& name, const std::string& texturePath, const bool flipVertical = false);

	/**
	* Retrieve a texture from the asset storage.
	* @param	name	The name of the texture you want to retrieve.
	* @return			The texture with corresponding name (if it exists).
	*/
	static Texture& GetTexture(const std::string& name);

	/**
	* Delete a texture from the asset storage.
	* @param	name	The name of the texture you want to delete.
	*/
	static void DeleteTexture(const std::string& name);



// -----------------------------------------------------------------------------
//                 Vertex Arrays
// -----------------------------------------------------------------------------

	/**
	* Create an unloaded vertex array and stores it.
	* @param	name			The name you want to give to this vertex array in the asset storage.
	*/
	static VertexArray& CreateVertexArray(const std::string& name);

	/**
	* Retrieve a vertex array from the asset storage.
	* @param	name	The name of the vertex array you want to retrieve.
	* @return			The vertex array with corresponding name (if it exists).
	*/
	static VertexArray& GetVertexArray(const std::string& name);

	/**
	* Delete a vertex array from the asset storage.
	* @param	name	The name of the vertex array you want to delete.
	*/
	static void DeleteVertexArray(const std::string& name);



// -----------------------------------------------------------------------------
//                 Meshes
// -----------------------------------------------------------------------------

	/**
	* Load a single mesh from handcoded vertices and stores it.
	* @param	name		The name you want to give to this mesh in the asset storage.
	* @param	vertices	The vertices of the mesh.
	* @param	indices		The indices of the mesh.
	*/
	static void LoadSingleMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});

	/**
	* Load a single mesh from file and stores it.
	* Warning: It will only load the root mesh.
	* @param	name		The name you want to give to this mesh in the asset storage.
	* @param	filepath	The path to the mesh file to read.
	*/
	static void LoadSingleMesh(const std::string& name, const std::string& filepath);

	/**
	* Load a mesh collection from file and stores it.
	* @param	name		The name you want to give to this mesh collection in the asset storage.
	* @param	filepath	The path to the mesh file to read.
	*/
	static void LoadMeshCollection(const std::string& name, const std::string& filepath);

	/**
	* Retrieve a single mesh from the asset storage.
	* @param	name	The name of the single mesh you want to retrieve.
	* @return			The single mesh with corresponding name (if it exists).
	*/
	static Mesh& GetSingleMesh(const std::string& name);

	/**
	* Retrieve a mesh collection from the asset storage.
	* @param	name	The name of the mesh collection you want to retrieve.
	* @return			The mesh collection with corresponding name (if it exists).
	*/
	static MeshCollection& GetMeshCollection(const std::string& name);

	/**
	* Delete a a single mesh from the asset storage.
	* @param	name	The name of the a single mesh you want to delete.
	*/
	static void DeleteSingleMesh(const std::string& name);

	/**
	* Delete a mesh collection from the asset storage.
	* @param	name	The name of the mesh collection you want to delete.
	*/
	static void DeleteMeshCollection(const std::string& name);



// -----------------------------------------------------------------------------
//                 Models
// -----------------------------------------------------------------------------

	/**
	* Create a model and stores it.
	* @param	name	The name you want to give to this model in the asset storage.
	* @return			The newly created model.
	*/
	static Model& CreateModel(const std::string& name);

	/**
	* Retrieve a model from the asset storage.
	* @param	name	The name of the model you want to retrieve.
	* @return			The model with corresponding name (if it exists).
	*/
	static Model& GetModel(const std::string& name);

	/**
	* Delete a model from the asset storage.
	* @param	name	The name of the model you want to delete.
	*/
	static void DeleteModel(const std::string& name);



// -----------------------------------------------------------------------------
//                 Shaders
// -----------------------------------------------------------------------------

	/**
	* Create a shader and stores it.
	* @param	name			The name you want to give to this shader in the asset storage.
	* @param	vertexName		The name of the vertex shader to bind in this program.
	* @param	fragmentName	The name of the fragment shader to bind in this program.
	* @param	shaderType		The type of this shader.
	* @return					The newly created shader.
	*/
	static void CreateShaderProgram(const std::string& name, const std::string& vertexName, const std::string& fragmentName, const ShaderType shaderType);

	/**
	* Retrieve a shader from the asset storage.
	* @param	name	The name of the shader you want to retrieve.
	* @return			The shader with corresponding name (if it exists).
	*/
	static Shader& GetShader(const std::string& name);

	/**
	* Delete a shader from the asset storage.
	* @param	name	The name of the shader you want to delete.
	*/
	static void DeleteShader(const std::string& name);



// -----------------------------------------------------------------------------
//                 Materials
// -----------------------------------------------------------------------------

	/**
	* Create a material with a shader and stores it.
	* @param	name		The name you want to give to this material in the asset storage.
	* @param	shaderUsed	The shader to use with the material.
	* @return				The newly created material.
	*/
	static Material& CreateMaterial(const std::string& name, Shader& shaderUsed);

	/**
	* Create a material collection in the storage.
	* @param	name				The name you want to give to this material collection in the asset storage.
	* @param	materialCollection	The materials to set in this collection.
	* @return						The newly created material collection.
	*/
	static MaterialCollection& CreateMaterialCollection(const std::string& name, const std::vector<Material*>& materialCollection);

	/**
	* Retrieve a material from the asset storage.
	* @param	name	The name of the material you want to retrieve.
	* @return			The material with corresponding name (if it exists).
	*/
	static Material& GetMaterial(const std::string& name);

	/**
	* Retrieve a material collection from the asset storage.
	* @param	name	The name of the material collection you want to retrieve.
	* @return			The material collection with corresponding name (if it exists).
	*/
	static MaterialCollection& GetMaterialCollection(const std::string& name);

	/**
	* Delete a material from the asset storage.
	* @param	name	The name of the material you want to delete.
	*/
	static void DeleteMaterial(const std::string& name);



// -----------------------------------------------------------------------------
//                 Fonts
// -----------------------------------------------------------------------------

	/**
	* Load a font from file and stores it.
	* @param	name				The name you want to give to this font in the asset storage.
	* @param	fontPath			The path to the font file to read.
	* @param	size				The size to load the font with.
	* @param	charLoadSetting		How many chars to load with this font.
	*/
	static void LoadFont(const std::string& name, const std::string& fontPath, const int size, const CharacterLoading charLoadSetting);

	/**
	* Retrieve a font from the asset storage.
	* @param	name	The name of the font you want to retrieve.
	* @return			The font with corresponding name (if it exists).
	*/
	static Font& GetFont(const std::string& name);

	/**
	* Delete a font from the asset storage.
	* @param	name	The name of the font you want to delete.
	*/
	static void DeleteFont(const std::string& name);



// -----------------------------------------------------------------------------
//                 Sounds
// -----------------------------------------------------------------------------

	/**
	* Create a sound from file and stores it.
	* @param	name		The name you want to give to this sound in the asset storage.
	* @param	filePath	The path to the sound file to read.
	* @param	settings	(optionnal) Additionnal load settings for the sound.
	* @return				The newly created sound.
	*/
	static AudioSound& CreateSound(const std::string& name, const std::string& filePath, const SoundSettings settings);

	/**
	* Retrieve a sound from the asset storage.
	* @param	name	The name of the sound you want to retrieve.
	* @return			The sound with corresponding name (if it exists).
	*/
	static AudioSound& GetSound(const std::string& name);

	/**
	* Delete a sound from the asset storage.
	* @param	name	The name of the sound you want to delete.
	*/
	static void DeleteSound(const std::string& name);



// -----------------------------------------------------------------------------
//                 Audio Collision Type
// -----------------------------------------------------------------------------

	/**
	* Register an audio collision type in the asset storage.
	* @param	name					The name you want to give to this audio collision type in the asset storage.
	* @param	audioCollisionType		The value of the audio collision type you want to register.
	* @return							The registered audio collision type.
	*/
	static AudioCollisionOcclusion& RegisterAudioCollisionType(const std::string& name, AudioCollisionOcclusion audioCollisionType);

	/**
	* Retrieve an audio collision type from the asset storage.
	* @param	name	The name of the audio collision type you want to retrieve.
	* @return			The audio collision type with corresponding name (if it exists).
	*/
	static AudioCollisionOcclusion& GetAudioCollisionType(const std::string& name);

	/**
	* Delete an audio collision type from the asset storage.
	* @param	name	The name of the audio collision type you want to delete.
	*/
	static void DeleteAudioCollisionType(const std::string& name);




// -----------------------------------------------------------------------------
//                 Assets lists
// -----------------------------------------------------------------------------

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
	static std::unordered_map<std::string, std::unique_ptr<VertexArray>> vertexArrays;
	static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshesSingle;
	static std::unordered_map<std::string, std::unique_ptr<MeshCollection>> meshesCollection;
	static std::unordered_map<std::string, std::unique_ptr<Model>> models;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
	static std::unordered_map<std::string, std::unique_ptr<Material>> materials;
	static std::unordered_map<std::string, std::unique_ptr<MaterialCollection>> materialsCollection;
	static std::unordered_map<std::string, std::unique_ptr<Font>> fonts;
	static std::unordered_map<std::string, std::unique_ptr<AudioSound>> sounds;
	static std::unordered_map<std::string, AudioCollisionOcclusion> audioCollisionTypes;
};

