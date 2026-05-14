#pragma once
#include "hudComponent.h"

class Texture;

/** Sprite Renderer Component
* Component for rendering a 2D sprite on the screen.
* This component doesn't use the transform of the Entity.
*/
class SpriteRendererComponent : public HudComponent
{
public:
	void setSpriteDatas(const Texture& spriteTexture_, const Vector2& screenAnchor_, const Vector2& pivot_, const Vector2& pos_, const Vector2& scale_, const float rotAngle_, const Color& tintColor_);

	void setSpriteTexture(const Texture& spriteTexture_);
	const Texture& getSpriteTexture() const;

	Vector2 getSize() const override;

	bool canDraw() const;
	
protected:
	virtual void registerComponent() override;
	virtual void unregisterComponent() override;

	void init() override;
	void exit() override;

	bool needToComputeMatrix() const override;

private:
	const Texture* spriteTexture{ nullptr };
};