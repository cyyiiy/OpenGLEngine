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
	void setModel(std::shared_ptr<Model> newModel);

	/** Change a material used by all the meshes of the model that uses the given material ID. */
	void setMaterial(std::shared_ptr<Material> material, MaterialID materialId);

	/** Reset the materials of this component to the defaults of its model. */
	void resetToDefaultMaterials();


	/** Check if this component uses the given material. */
	bool usesMaterial(const Material* material) const;

	/** Return true if this component has a valid model. */
	bool isValid() const;


	/** Iterates on the meshes that uses the given material.
	* 
	* Usage: 
	* 
	* component.ForEachMeshOfMaterial(material, [this](const Mesh& mesh) { this->foo(mesh); });
	* 
	* @param	material	The material to check.
	* @param	func		The lambda to execute for each mesh.
	*/
	template <typename Func>
	void ForEachMeshOfMaterial(const Material* material, Func&& func) const
	{
		for (size_t mat_id = 0; mat_id < materials.size(); ++mat_id)
		{
			if (materials[mat_id].get() != material) continue;
			for (uint32_t mesh_id : model->getMeshIdsForMaterialId(static_cast<MaterialID>(mat_id)))
				func(model->getMesh(mesh_id));
		}
	}


	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The transform offset of this Model Renderer Component. */
	Transform offset;

private:
	std::shared_ptr<Model> model{ nullptr };
	std::vector<std::shared_ptr<Material>> materials;
};