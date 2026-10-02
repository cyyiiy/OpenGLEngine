#pragma once
#include <ECS/entity.h>
#include <Maths/Vector2.h>

class EntityContainer;


namespace WallFactory
{
	enum class WallFacingDirection : uint8_t
	{
		WallFacingPositiveX = 0,
		WallFacingNegativeX = 1,
		WallFacingPositiveZ = 2,
		WallFacingNegativeZ = 3
	};

	Entity* CreateWall(EntityContainer* entityContainer, WallFacingDirection facingDirection, const Vector3& position, const Vector2& scale, bool isAltTex, bool createCollision = true);

	void SetupWallAssets();
}