#include "renderTargetTexture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Maths/Vector2Int.h>
#include <ServiceLocator/locator.h>


RenderTargetTexture::RenderTargetTexture(unsigned int _framebuffer, unsigned int _colorTexture, unsigned int _depthBuffer, int _width, int _height) :
	IAsset(), framebuffer(_framebuffer), colorTexture(_colorTexture), depthBuffer(_depthBuffer),
	width(_width), height(_height)
{}

RenderTargetTexture::~RenderTargetTexture()
{
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &colorTexture);
	glDeleteRenderbuffers(1, &depthBuffer);
}

std::string RenderTargetTexture::GetTypeName()
{
	return "Render Target Texture";
}

std::shared_ptr<RenderTargetTexture> RenderTargetTexture::Create(const LoadParams& params)
{
	// Initialize the opengl ids and retrieve the size from loading params
	unsigned int framebuffer = 0;
	unsigned int colorTexture = 0;
	unsigned int depthBuffer = 0;

	const int width = params.width;
	const int height = params.height;

	// Create the color texture
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Set the texture wrapping and filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create the frame buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	// Create the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Check the frame buffer has been created correctly
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Locator::getLog().LogMessage_Category("Render Target Texture: An error occured while creating the framebuffer.", LogCategory::Error);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return nullptr;
	}

	// Unbind the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return std::make_shared<RenderTargetTexture>(framebuffer, colorTexture, depthBuffer, width, height);
}

RenderTargetTexture::LoadParams RenderTargetTexture::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}

uint64_t RenderTargetTexture::getAssetMemorySize() const
{
	return sizeof(RenderTargetTexture);
}

uint64_t RenderTargetTexture::getAssetGpuSize() const
{
	const uint64_t texture_size = width * height * 4; // 4 * 8 bits (GL_RGBA8)
	const uint64_t buffer_size = width * height * 4; // 32 bits (GL_DEPTH24_STENCIL8)
	return texture_size + buffer_size;
}

void RenderTargetTexture::use() const
{
	if (binded)
	{
		// Prevent reading from colorTexture while the framebuffer is being written on
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, colorTexture);
	}
}

void RenderTargetTexture::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	binded = true;
}

void RenderTargetTexture::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	binded = false;
}

void RenderTargetTexture::resize(int _width, int _height)
{
	// Check the new size
	if (_width <= 0 || _height <= 0) return;
	if (_width == width || _height == height) return;

	width = _width;
	height = _height;

	// Resize the color texture
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Resize the depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	// Unbind the opengl ids
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Vector2Int RenderTargetTexture::getTextureSize() const
{
	return Vector2Int{ width, height };
}
