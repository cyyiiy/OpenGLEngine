#include "spriteRendererComponent.h"
#include <Rendering/texture.h>
#include <Maths/Vector2Int.h>
#include <ServiceLocator/locator.h>
#include <ServiceLocator/renderer.h>


void SpriteRendererComponent::setSpriteDatas(const Texture& spriteTexture_, const Vector2& screenAnchor_, const Vector2& pivot_, const Vector2& pos_, const Vector2& scale_, const float rotAngle_, const Color& tintColor_)
{
	spriteTexture = &spriteTexture_;

	setHudTransform(screenAnchor_, pivot_, pos_, scale_, rotAngle_);
	setTintColor(tintColor_);
}

void SpriteRendererComponent::setSpriteTexture(const Texture& spriteTexture_)
{
	spriteTexture = &spriteTexture_;
}

const Texture& SpriteRendererComponent::getSpriteTexture() const
{
	return *spriteTexture;
}

Vector2 SpriteRendererComponent::getSize() const
{
	if (!spriteTexture) return getScale();

	return spriteTexture->getTextureSize() * getScale();
}

bool SpriteRendererComponent::canDraw() const
{
	return spriteTexture && getEnabled();
}

void SpriteRendererComponent::registerComponent()
{
	Locator::getRenderer().AddSprite(this);
}

void SpriteRendererComponent::unregisterComponent()
{
	Locator::getRenderer().RemoveSprite(this);
}

void SpriteRendererComponent::init()
{
	//  reset the values in case this component was used before (the component manager is a memory pool)
	resetValues();
	spriteTexture = nullptr;


	bindScreenResize();
}

void SpriteRendererComponent::exit()
{
	unbindScreenResize();
}

bool SpriteRendererComponent::needToComputeMatrix() const
{
	return true;
}
