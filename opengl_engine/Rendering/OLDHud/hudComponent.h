#pragma once
#include <ECS/component.h>
#include <Events/observer.h>

#include <Utils/color.h>
#include <Maths/Vector2.h>
#include <Maths/Matrix4.h>


/** Hud Component
* Base class for specific hud components.
* Abstract Component - do not add it to an Entity.
*/
class HudComponent : public Component, public Observer
{
public:
	virtual ~HudComponent() {} //  hud component has a pure virtual function, therefore it's an interface

	void setEnabled(const bool enabled_);
	bool getEnabled() const;

	void setHudTransform(const Vector2& screenAnchor_, const Vector2& pivot_, const Vector2& pos_, const Vector2& scale_, const float rotAngle_);
	void setScreenAnchor(const Vector2& screenAnchor_);
	void setPivot(const Vector2& pivot_);
	void setPos(const Vector2& pos_);
	void setScale(const Vector2& scale_);
	void setRotAngle(const float rotAngle_);
	void setTintColor(const Color& tintColor_);

	Vector2 getPivot() const;
	Vector2 getPos() const;
	Vector2 getScale() const;
	float getRotAngle() const;
	Color getTintColor() const;

	/* Hud elements size is often affected by something else that scale (for exemple, texture size for sprites). This function must be overriden in every class that inherit hud. */
	virtual Vector2 getSize() const = 0;
	Vector2 getScreenPos() const;
	Matrix4 getHudTransform() const;

protected:
	/* Does the hud component need to compute the transformation matrix? (For exemple, texts doesn't need to.) This function must be overriden in every class that inherit hud. */
	virtual bool needToComputeMatrix() const = 0;

	void bindScreenResize();
	void unbindScreenResize();

	void resetValues();

private:
	bool enabled{ true };

	Vector2 screenAnchor{ Vector2::halfUnit };
	Vector2 pivot{ Vector2::halfUnit };
	Vector2 pos{ Vector2::zero };
	Vector2 scale{ Vector2::one };
	float rotAngle{ 0.0f };
	Color tintColor{ Color::white };

	Vector2 screenPos{ Vector2::zero };
	Matrix4 hudTransform{ Matrix4::identity };

	void updatePosWithAnchor();
	void computeMatrix();

	void onWindowResizeEvent(const Vector2Int windowSize);
};

