#pragma once
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>
#include <filesystem>
#include <string>


class Texture : public IAsset
{
public:
	// Asset part
	struct LoadParams
	{
		std::filesystem::path texturePath;
		bool flipVertical;

		// Note: Wrapping and Filtering Parameters can be added here later
		// They would replace their default values being initialized in `Create`
	};

	Texture(unsigned int _ID, int _width, int _height);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<Texture> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;


	// Texture part
	void use() const;
	unsigned int getTextureID() const { return ID; }

	int getTextureWidth() const { return width; }
	int getTextureHeight() const { return height; }
	struct Vector2Int getTextureSize() const;

private:
	unsigned int ID{ 0 };
	int width{ 0 };
	int height{ 0 };

	static unsigned int GetGlFormat(const int nbChannels);
};