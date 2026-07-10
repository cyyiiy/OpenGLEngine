#pragma once
#include <ECS/component.h>
#include <Rendering/Model/model.h>
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
	void setModel(Model* newModel);

	/** Change a material used by all the meshes of the model that uses the given material ID. */
	void setMaterial(Material* material, int materialId);

	/** Reset the materials of this component to the defaults of its model. */
	void resetToDefaultMaterials();

	/** Get all the meshes of the model that are drawn with the given material. */
	const std::vector<std::shared_ptr<Mesh>> retrieveMeshesOfMaterial(const Material& material) const;

	/** Know if this component uses the given material. */
	bool usesMaterial(const Material& material) const;

	/** Return true if this component has a valid model. */
	bool isValid() const;


	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The transform offset of this Model Renderer Component. */
	Transform offset;

private:
	Model* model{ nullptr };
	std::unordered_map<MaterialUniqueID, std::vector<int>> materials;

	MaterialUniqueID getMaterialUniqueID(const Material* material) const;
};