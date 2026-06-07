#include "benchmarkECS.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Rendering/Text/textComponent.h>
#include <Components/benchmarkComponentA.h>


void BenchmarkECS::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color::black);

	// Create some text to explain this benchmark
	Entity* text = createEntity();

	TextComponent& text_comp_1 = ECS::GetComponent(text->addComponentByClass<TextComponent>());
	text_comp_1.setTextDatas("ECS Benchmark", AssetManager::GetFont("arial_64"));
	text_comp_1.position = HudPosition{ Vector2{ 0.5f, 1.0f }, Vector2{ 0.5f, 1.0f }, Vector2{ 0.0f, -40.0f } };

	TextComponent& text_comp_2 = ECS::GetComponent(text->addComponentByClass<TextComponent>());
	text_comp_2.setTextDatas(
		"Test ECS functions 'Create', 'Get', 'Delete' and 'Update'\n"
		"There are 1000 components. Each frame, they create another component, get it,\n"
		"make it add a random number to a shared counter then finally delete the component.",
		AssetManager::GetFont("arial_64"));
	text_comp_2.position = HudPosition{ Vector2{ 0.5f, 1.0f }, Vector2{ 0.0f, 1.0f }, Vector2{ 25.0f, -200.0f } };

	// Create text to show the shared counter
	counterText = text->addComponentByClass<TextComponent>();
	TextComponent& counter_text_comp = ECS::GetComponent(counterText);
	counter_text_comp.setTextDatas("Shared counter value for this frame: 0", AssetManager::GetFont("arial_24"));
	counter_text_comp.position = HudPosition{ Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f }, Vector2{ 25.0f, -400.0f } };
	counter_text_comp.tintColor = Color::cyan;

	// Create entities
	dummyEntity = createEntity();
	Entity* comp_entity = createEntity();

	// Create 1000 benchmark components
	for (int i = 0; i < 1000; i++)
	{
		BenchmarkComponentA& comp = ECS::GetComponent(comp_entity->addComponentByClass<BenchmarkComponentA>());
		comp.setPointers(dummyEntity, &sharedCounter);
	}
}

void BenchmarkECS::unloadScene()
{
}

void BenchmarkECS::updateScene(float dt)
{
	ECS::GetComponent(counterText).setText("Shared counter value for this frame: " + std::to_string(sharedCounter));
	sharedCounter = 0;
}
