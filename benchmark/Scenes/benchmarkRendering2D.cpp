#include "benchmarkRendering2D.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/Text/textRendererComponent.h>
#include <Rendering/Hud/spriteRendererComponent.h>


void BenchmarkRendering2D::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color::black);

	// Create 600 static texts
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			Entity* text = createEntity();
			std::shared_ptr<TextRendererComponent> text_comp = text->addComponentByClass<TextRendererComponent>();
			text_comp->setTextDatas("Benchmark", AssetManager::GetFont("arial_24"), Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f },
				Vector2{ x * 95.0f, -y * 20.0f }, Vector2{ 0.7f }, 0.0f, Color::white);
		}
	}

	// Create 40 sprites
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			Entity* sprite = createEntity();
			std::shared_ptr<SpriteRendererComponent> sprite_comp = sprite->addComponentByClass<SpriteRendererComponent>();
			sprite_comp->setSpriteDatas(AssetManager::GetTexture("sprite_matrix"), Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f },
				Vector2{ x * 95.0f, -700.0f - y * 95.0f }, Vector2{ 0.15f }, 0.0f, Color::white);
		}
	}

	// Create 10 spinning texts
	for (int x = 0; x < 10; x++)
	{
		Entity* spinning_text = createEntity();
		std::shared_ptr<TextRendererComponent> spinning_text_comp = spinning_text->addComponentByClass<TextRendererComponent>();
		spinning_text_comp->setTextDatas("Benchmark", AssetManager::GetFont("arial_24"), Vector2{ 0.0f, 1.0f }, Vector2::halfUnit,
			Vector2{ 130.0f + x * 180.0f, -950.0f }, Vector2{ 1.2f }, 0.0f, Color::white);
		spinning_texts.push_back(spinning_text_comp);
	}
}

void BenchmarkRendering2D::unloadScene()
{
}

void BenchmarkRendering2D::updateScene(float dt)
{
	for (auto& spinning_text_comp : spinning_texts)
	{
		spinning_text_comp->setRotAngle(Maths::fmod(spinning_text_comp->getRotAngle() + dt * 180.0f, 360.0f));
	}
}
