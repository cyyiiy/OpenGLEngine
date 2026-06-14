#include "benchmarkEnd.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Rendering/Text/textComponent.h>


void BenchmarkEnd::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color::black);

	Entity* text = createEntity();
	TextComponent& text_comp = ECS::GetComponent(text->addComponentByClass<TextComponent>());
	text_comp.setTextDatas("Benchmark is finished.\nRead log to see results and press escape to close.", AssetManager::GetFont("arial_64"));
}

void BenchmarkEnd::unloadScene()
{
}
