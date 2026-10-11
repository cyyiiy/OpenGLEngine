#pragma once
#include <ECS/component.h>
#include <Rendering/Model/model.h>
#include <Rendering/material.h>
#include <Core/transform.h>

#include <unordered_map>
#include <vector>
#include <memory>


/** Model Renderer Component
* A component that allows to render a 3D model.
*/
class ModelRendererComponent : public Component
{
public:
	/** Set the model rendered by this component. This will set the materials of this component to the defaults of the new model. */
	void setModel(std::shared_ptr<Model> newModel);

	/** Change a material used by all the meshes of the model that uses the given material ID. */
	void setMaterial(std::shared_ptr<Material> material, MaterialID materialId);

	/** Reset the materials of this component to the defaults of its model. */
	void resetToDefaultMaterials();


	/** Check if this component uses the given shader. */
	bool usesShader(const Shader* shader) const;

	/** Return true if this component has a valid model. */
	bool isValid() const;

	/** Get the model used by this component. */
	const Model& getModel() const { return *model; }

	/** Get the list of materials used by this component. */
	const std::vector<std::shared_ptr<Material>>& getMaterials() const { return materials; }


	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The transform offset of this Model Renderer Component. */
	Transform offset;

private:
	std::shared_ptr<Model> model{ nullptr };
	std::vector<std::shared_ptr<Material>> materials;
};