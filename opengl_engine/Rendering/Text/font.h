#pragma once
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>
#include <Rendering/Text/textRenderUtils.h>
#include <unordered_map>
#include <filesystem>


class Font : public IAsset
{
public:
	// Asset part
	struct LoadParams
	{
		std::filesystem::path fontPath;
		int fontSize;
		CharacterLoading charSettings;
	};

	Font(int _fontSize, std::unordered_map<char, FontCharacter> _fontCharacters, unsigned int _textureArrayID);
	~Font();

	Font(const Font&) = delete;
	Font(Font&&) = delete;
	Font& operator=(const Font&) = delete;
	Font& operator=(Font&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<Font> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// Font part
	void use() const;
	const int getFontSize() const;
	const FontCharacter& getCharacter(const char& c) const;


private:
	int fontSize;
	std::unordered_map<char, FontCharacter> fontCharacters;
	unsigned int textureArrayID{ 0 };
};