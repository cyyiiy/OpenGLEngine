#pragma once

class TextureInterface
{
public:
	virtual ~TextureInterface() = default;

	virtual void use() const = 0;

	virtual struct Vector2Int getTextureSize() const = 0;
};