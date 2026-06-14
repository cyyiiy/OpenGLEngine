#include "raycastUtils.h"
#include "boxCollisionComponent.h"

RaycastHitInfos RaycastHitInfos::defaultInfos(Vector3::zero, Vector3::zero, std::numeric_limits<float>::max(), ComponentHandle<BoxCollisionComponent>(), {});