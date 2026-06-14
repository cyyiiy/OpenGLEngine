#pragma once
#include <ECS/component.h>
#include <Rendering/Model/model.h>
#include <Core/transform.h>


/** Model Renderer Component
* A data-only component that holds informations to render a 3D model.
*/
class ModelRendererComponent : public Component
{
public:
	/** The 3D model to render. */
	Model* model{ nullptr };

	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The transform offset of this Model Renderer Component. */
	Transform offset;
};