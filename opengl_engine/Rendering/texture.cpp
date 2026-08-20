#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Maths/Vector2Int.h>
#include <ServiceLocator/locator.h>
#include <Utils/defines.h>

// Define anisotropic filtering since it's not defined in glad
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF



Texture::Texture(unsigned int _ID, int _width, int _height) : 
	IAsset(), ID(_ID), width(_width), height(_height)
{}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}


std::string Texture::GetTypeName()
{
	return "Texture";
}

std::shared_ptr<Texture> Texture::Create(const LoadParams& params)
{
	// Initialize the texture id and the texture path
	unsigned int id;
	int width, height;
	std::filesystem::path tex_path = RESOURCES_PATH;
	tex_path += params.texturePath;

	// Create the texture in OpenGL
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Set the texture wrapping and filtering parameters (default values for now)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	int nb_channels;
	stbi_set_flip_vertically_on_load(params.flipVertical);
	unsigned char* data = stbi_load(tex_path.string().c_str(), &width, &height, &nb_channels, 0);

	if (data)
	{
		unsigned int src_format = GetSrcFormat(nb_channels);
		unsigned int gl_format = GetGlFormat(nb_channels);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, src_format, GL_UNSIGNED_BYTE, data);

		// Note: In some cases, the glGenerateMipmap function can cause crashes (it's related to the size of the image, but I don't know exactly what causes this problem)
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the anisotropy
		GLfloat max_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Maths::clamp(max_anisotropy, 0.0f, 16.0f));

		stbi_image_free(data);
	}
	else
	{
		Locator::getLog().LogMessage_Category("Texture: Failed to load texture at path " + tex_path.string() + ".", LogCategory::Error);
		return nullptr;
	}

	return std::make_shared<Texture>(id, width, height);
}

Texture::LoadParams Texture::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}

uint64_t Texture::getAssetMemorySize() const
{
	return sizeof(unsigned int) + 2 * sizeof(int);
}


void Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

Vector2Int Texture::getTextureSize() const
{
	return Vector2Int{ width, height };
}


unsigned int Texture::GetSrcFormat(const int nbChannels)
{
	switch (nbChannels)
	{
	case 1:
		return GL_RED;

	case 2:
		return GL_RG;

	case 3:
		return GL_RGB;

	default:
		return GL_RGBA;
	}
}

unsigned int Texture::GetGlFormat(const int nbChannels)
{
	switch (nbChannels)
	{
	case 1:
		return GL_R8;

	case 2:
		return GL_RG8;

	case 3:
		return GL_RGB8;

	default:
		return GL_RGBA8;
	}
}