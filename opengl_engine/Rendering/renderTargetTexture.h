#pragma once
#include "textureInterface.h"
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>


class RenderTargetTexture : public TextureInterface, public IAsset
{
public:
	// Asset part
	struct LoadParams
	{
		int width;
		int height;
	};

	RenderTargetTexture(unsigned int _framebuffer, unsigned int _colorTexture, unsigned int _depthBuffer, int _width, int _height);
	~RenderTargetTexture();

	RenderTargetTexture(const RenderTargetTexture&) = delete;
	RenderTargetTexture(RenderTargetTexture&&) = delete;
	RenderTargetTexture& operator=(const RenderTargetTexture&) = delete;
	RenderTargetTexture& operator=(RenderTargetTexture&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<RenderTargetTexture> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// RenderTargetTexture part
	void use() const override;
	void bind() const;
	void unbind() const;
	void resize(int _width, int _height);

	unsigned int getColorTexture() const { return colorTexture; }

	int getTextureWidth() const { return width; }
	int getTextureHeight() const { return height; }
	struct Vector2Int getTextureSize() const override;

private:
	unsigned int framebuffer{ 0 };
	unsigned int colorTexture{ 0 };
	unsigned int depthBuffer{ 0 };

	int width{ 0 };
	int height{ 0 };

	mutable bool binded{ false };
};

