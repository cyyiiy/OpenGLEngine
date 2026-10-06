#include "font.h"
#include <glad/glad.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <ServiceLocator/locator.h>
#include <Utils/memoryUtils.h>
#include <Utils/defines.h>


Font::Font(int _fontSize, std::unordered_map<char, FontCharacter> _fontCharacters, unsigned int _textureArrayID) :
	IAsset(), fontSize(_fontSize), fontCharacters(std::move(_fontCharacters)), textureArrayID(_textureArrayID)
{}

Font::~Font()
{
	glDeleteTextures(1, &textureArrayID);
}


std::string Font::GetTypeName()
{
	return "Font";
}

std::shared_ptr<Font> Font::Create(const LoadParams& params)
{
	// Initialize freetype
	FT_Error error;
	FT_Library ft;
	error = FT_Init_FreeType(&ft);
	if (error)
	{
		Locator::getLog().LogMessage_Category("Font: Failed to initialize FreeType library.", LogCategory::Error);
		return nullptr;
	}

	// Initialize the loading variables
	unsigned int texture_array_id;
	std::unordered_map<char, FontCharacter> font_characters;
	std::filesystem::path font_path = RESOURCES_PATH;
	font_path += params.fontPath;

	// Load the font file
	FT_Face face;
	error = FT_New_Face(ft, font_path.string().c_str(), 0, &face);
	if (error)
	{
		Locator::getLog().LogMessage_Category("Font: Failed to load font at path " + font_path.string() + ".", LogCategory::Error);
		return nullptr;
	}

	// Set size to load individual glyphs
	FT_Set_Pixel_Sizes(face, params.fontSize, params.fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Convert char settings 
	int char_count;
	switch (params.charSettings)
	{
	case CharacterLoading::ASCII_128:
		char_count = 128;
		break;

	default:
		char_count = 128; // Choose ascii 128 if unknown setting
		break;
	}

	// Generate the texture array
	glGenTextures(1, &texture_array_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array_id);

	// Setup the texture 3D (this is the array of textures), here the font size are for the size of the textures and the char_count for the size of the array
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, params.fontSize, params.fontSize, char_count, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	// Set the parameters of the texture
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load individual characters
	for (unsigned char c = 0; c < char_count; c++)
	{
		// Load character glyph
		error = FT_Load_Char(face, c, FT_LOAD_RENDER);
		if (error)
		{
			Locator::getLog().LogMessage_Category("Font: Failed to load character glyph at char '" + std::string(1, c) + "'.", LogCategory::Error);
			continue;
		}

		// Add a texture for the current char into the texture array
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			0, 0, // Offset x & y
			int(c), // Offset z (the index to set the texture in the array)
			face->glyph->bitmap.width, // Size width
			face->glyph->bitmap.rows, // Size height
			1, // Size depth (leave at 1)
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer // Datas of the texture
		);

		// Store character for later use
		font_characters.emplace(c,
			FontCharacter{
				int(c), // Index of the texture in the texture array
				Vector2Int{ (int)(face->glyph->bitmap.width), (int)(face->glyph->bitmap.rows) },
				Vector2Int{ face->glyph->bitmap_left, face->glyph->bitmap_top },
				face->glyph->advance.x
			}
		);
	}

	// Reset OpenGL state
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Release freetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return std::make_shared<Font>(params.fontSize, font_characters, texture_array_id);
}

Font::LoadParams Font::ParseCyasset(const CyassetDocument& cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}


uint64_t Font::getAssetMemorySize() const
{
	return sizeof(Font) + MemoryUtils::EstimateUnorderedMapHeapMemory(fontCharacters);
}

uint64_t Font::getAssetGpuSize() const
{
	return (uint64_t)(fontSize * fontSize * fontCharacters.size());
}


void Font::use() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
}

const int Font::getFontSize() const
{
	return fontSize;
}

const FontCharacter& Font::getCharacter(const char& c) const
{
	if (c >= fontCharacters.size())
	{
		Locator::getLog().LogMessage_Category("Font: Tried to get a character outside of the char range loaded on this font.", LogCategory::Warning);
		return FontCharacter::errorChar;
	}

	return fontCharacters.at(c);
}