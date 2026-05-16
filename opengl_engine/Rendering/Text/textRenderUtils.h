#pragma once
#include <Maths/Vector2Int.h>
#include <Maths/Vector2.h>

struct FontCharacter
{
	int textureId;
	Vector2Int size;
	Vector2Int bearing;
	int advance;

	static FontCharacter errorChar;
};

enum class CharacterLoading : uint8_t
{
	ASCII_128 = 0 //  Load the first 128 characters of the ASCII system
};