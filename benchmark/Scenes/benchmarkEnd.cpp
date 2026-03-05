#include "benchmarkEnd.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Rendering/Text/textRendererComponent.h>


void BenchmarkEnd::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color::black);

	Entity* text = createEntity();
	std::shared_ptr<TextRendererComponent> text_comp = text->addComponentByClass<TextRendererComponent>();
	text_comp->setText("Benchmark is finished.\nRead log to see results and press escape to close.");
}

void BenchmarkEnd::unloadScene()
{
}
