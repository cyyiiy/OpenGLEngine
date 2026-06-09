#pragma once
#include <ECS/behaviorComponent.h>

class PointLightComponent;
class ModelRendererComponent;


class LampComponent : public BehaviorComponent
{
public:
	void setup(ComponentHandle<PointLightComponent> lightComp, ComponentHandle<ModelRendererComponent> modelRendererComp, bool setupChandelier);
	void changeStatus(bool lightOn);

protected:
	void update(float deltaTime) override;

private:
	ComponentHandle<PointLightComponent> light;
	ComponentHandle<ModelRendererComponent> modelRenderer;

	bool isChandelier{ false };
	bool compValid{ false };

	float timer{ 0.0f };
	bool reverse{ false };
	float baseLightIntensity{ 0.0f };
};


// Specify sublist size for 'LampComponent'
template<>
struct ComponentSublistSize<LampComponent>
{
	static constexpr size_t value = 8;
};