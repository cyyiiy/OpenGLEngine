#include "modelRendererComponent.h"
#include <ServiceLocator/locator.h>


void ModelRendererComponent::setModel(std::shared_ptr<Model> newModel)
{
	model = newModel;
	resetToDefaultMaterials();
}

void ModelRendererComponent::setMaterial(std::shared_ptr<Material> material, MaterialID materialId)
{
	if (!isValid())
	{
		Locator::getLog().LogMessage_Category("Model Renderer Component: Tried to change a material on a component that doesn't have a valid model.", LogCategory::Warning);
		return;
	}

	const size_t mat_id = static_cast<size_t>(materialId);
	if (mat_id >= materials.size())
	{
		Locator::getLog().LogMessage_Category("Model Renderer Component: Tried to change a material on a material ID that doesn't exist on its model.", LogCategory::Warning);
		return;
	}

	materials[mat_id] = material;
}

void ModelRendererComponent::resetToDefaultMaterials()
{
	if (!isValid())
	{
		materials.clear();
		return;
	}

	materials = model->getDefaultMaterials();
}

bool ModelRendererComponent::usesMaterial(const Material* material) const
{
	return std::any_of(materials.begin(), materials.end(),
		[material](const std::shared_ptr<Material>& mat) { return mat.get() == material; });
}

bool ModelRendererComponent::isValid() const
{
	return model != nullptr;
}