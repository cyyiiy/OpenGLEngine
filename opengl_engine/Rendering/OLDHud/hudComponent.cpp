#include "hudComponent.h"
#include <GameplayStatics/gameplayStatics.h>


void HudComponent::setEnabled(const bool enabled_)
{
	enabled = enabled_;
}

bool HudComponent::getEnabled() const
{
	return enabled;
}


void HudComponent::setHudTransform(const Vector2& screenAnchor_, const Vector2& pivot_, const Vector2& pos_, const Vector2& scale_, const float rotAngle_)
{
	screenAnchor = screenAnchor_;
	pos = pos_;
	scale = scale_;
	rotAngle = rotAngle_;
	setPivot(pivot_); //  setPivot function will clamp pivot and recompute the screen pos and the transform matrix
}

void HudComponent::setScreenAnchor(const Vector2& screenAnchor_)
{
	screenAnchor = screenAnchor_;

	updatePosWithAnchor();
	computeMatrix();
}

void HudComponent::setPivot(const Vector2& pivot_)
{
	pivot = Vector2{ Maths::clamp<float>(pivot_.x, 0.0f, 1.0f), Maths::clamp<float>(pivot_.y, 0.0f, 1.0f) };

	updatePosWithAnchor();
	computeMatrix();
}

void HudComponent::setPos(const Vector2& pos_)
{
	pos = pos_;

	updatePosWithAnchor();
	computeMatrix();
}

void HudComponent::setScale(const Vector2& scale_)
{
	scale = scale_;

	computeMatrix();
}

void HudComponent::setRotAngle(const float rotAngle_)
{
	rotAngle = rotAngle_;

	computeMatrix();
}

void HudComponent::setTintColor(const Color& tintColor_)
{
	tintColor = tintColor_;
}


Vector2 HudComponent::getPivot() const
{
	return pivot;
}

Vector2 HudComponent::getPos() const
{
	return pos;
}

Vector2 HudComponent::getScale() const
{
	return scale;
}

float HudComponent::getRotAngle() const
{
	return rotAngle;
}

Color HudComponent::getTintColor() const
{
	return tintColor;
}

Vector2 HudComponent::getScreenPos() const
{
	return screenPos;
}

Matrix4 HudComponent::getHudTransform() const
{
	return hudTransform;
}

void HudComponent::bindScreenResize()
{
	GameplayStatics::OnScrenResize.registerObserver(this, Bind_1(&HudComponent::onWindowResizeEvent));
}

void HudComponent::unbindScreenResize()
{
	GameplayStatics::OnScrenResize.unregisterObserver(this);
}

void HudComponent::resetValues()
{
	//  reset the values in case this component was used before (the component manager is a memory pool) | called from init in derived classes
	enabled = true;
	screenAnchor = Vector2::halfUnit;
	pivot = Vector2::halfUnit;
	pos = Vector2::zero;
	scale = Vector2::one;
	rotAngle = 0.0f;
	tintColor = Color::white;
	screenPos = Vector2::zero;
	hudTransform = Matrix4::identity;
}




void HudComponent::updatePosWithAnchor()
{
	const Vector2Int window_size = GameplayStatics::GetWindowSize();

	screenPos = (window_size * (screenAnchor - Vector2{ 0.5f })) + pos;
}

void HudComponent::computeMatrix()
{
	if (!needToComputeMatrix()) return;

	const Vector2 size = getSize();
	const Vector2 pivot_inv_y = Vector2{ pivot.x, 1.0f - pivot.y };

	hudTransform =
		Matrix4::createScale(Vector3{ size, 1.0f }) * //  scale to the size of the hud element
		Matrix4::createTranslation(size * -pivot_inv_y) * //  translate to use the pivot
		Matrix4::createRotationZ(Maths::toRadians(rotAngle)) * //  rotate the hud element
		Matrix4::createTranslation(screenPos); //  translate to the position of the hud element
}

void HudComponent::onWindowResizeEvent(const Vector2Int windowSize)
{
	updatePosWithAnchor();
}
