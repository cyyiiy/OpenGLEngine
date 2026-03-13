#include "benchmarkECS.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Rendering/Text/textRendererComponent.h>
#include <Components/benchmarkComponentA.h>


void BenchmarkECS::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color::black);

	// Create some text to explain this benchmark
	Entity* text = createEntity();

	text->addComponentByClass<TextRendererComponent>()->setTextDatas(
		"ECS Benchmark", 
		AssetManager::GetFont("arial_64"), Vector2{ 0.5f, 1.0f }, Vector2{ 0.5f, 1.0f },
		Vector2{ 0.0f, -40.0f }, Vector2{ 1.0f }, 0.0f, Color::white
	);

	text->addComponentByClass<TextRendererComponent>()->setTextDatas(
		"Test ECS functions 'Create', 'Get', 'Delete' and 'Update'\n"
		"There are 1000 components. Each frame, they create another component, get it,\n"
		"make it add a random number to a shared counter then finally delete the component.",
		AssetManager::GetFont("arial_24"), Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f },
		Vector2{ 25.0f, -200.0f }, Vector2{ 1.0f }, 0.0f, Color::white
	);

	// Create text to show the shared counter
	counterText = text->addComponentByClass<TextRendererComponent>();
	counterText->setTextDatas(
		"Shared counter value for this frame: 0",
		AssetManager::GetFont("arial_24"), Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f },
		Vector2{ 25.0f, -400.0f }, Vector2{ 1.0f }, 0.0f, Color::cyan
	);

	// Create entities
	dummyEntity = createEntity();
	Entity* comp_entity = createEntity();

	// Create 1000 benchmark components
	for (int i = 0; i < 1000; i++)
	{
		std::shared_ptr<BenchmarkComponentA> comp = comp_entity->addComponentByClass<BenchmarkComponentA>();
		comp->SetPointers(dummyEntity, &sharedCounter);
	}
}

void BenchmarkECS::unloadScene()
{
}

void BenchmarkECS::updateScene(float dt)
{
	counterText->setText("Shared counter value for this frame: " + std::to_string(sharedCounter));
	sharedCounter = 0;
}
