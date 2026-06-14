#pragma once
#include <Maths/Vector2.h>
#include <Utils/Color.h>


/** Hud Position
* Set of values that affect the drawn position of the hud element on the screen.
*/
struct HudPosition
{
	/** Where the hud element will be placed in the screen. | (0,0) is bottom left corner and (1,1) is top right corner. */
	Vector2 screenAnchor;

	/** Pivot of the hud element, around which the rotation will be calculated. | (0.5,0.5) is the center of the hud element. */
	Vector2 pivot;

	/** Offset from the screen anchor computed position. */
	Vector2 offset;
};


/** Hud Element
* Shared base values for specific hud components.
*/
class HudElement
{
public:
	/** Wether or not the hud element is drawn. */
	bool active{ true };

	/** Where the hud element will be drawn in the screen. Defaults to screen center.  */
	HudPosition position{ Vector2::halfUnit, Vector2::halfUnit, Vector2::zero };

	/** Scale of the hud element. */
	Vector2 scale{ Vector2::one };

	/** Rotation around the pivot point of the hud element. In degrees. */
	float rotAngle{ 0.0f };

	/** The tint color of the hud element. */
	Color tintColor{ Color::white };
};