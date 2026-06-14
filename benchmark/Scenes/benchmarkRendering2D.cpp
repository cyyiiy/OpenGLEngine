#include "benchmarkRendering2D.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/Text/textComponent.h>
#include <Rendering/Hud/spriteComponent.h>


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
			TextComponent& text_comp = ECS::GetComponent(text->addComponentByClass<TextComponent>());
			text_comp.setTextDatas("Benchmmark", AssetManager::GetFont("arial_24"));
			text_comp.position = HudPosition{ Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f }, Vector2{ x * 95.0f, -y * 20.0f } };
			text_comp.scale = Vector2{ 0.7f };
		}
	}

	// Create 40 sprites
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			Entity* sprite = createEntity();
			SpriteComponent& sprite_comp = ECS::GetComponent(sprite->addComponentByClass<SpriteComponent>());
			sprite_comp.texture = &AssetManager::GetTexture("sprite_matrix");
			sprite_comp.position = HudPosition{ Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f }, Vector2{ x * 95.0f, -700.0f - y * 95.0f } };
			sprite_comp.scale = Vector2{ 0.15f };
		}
	}

	// Create 10 spinning texts
	for (int x = 0; x < 10; x++)
	{
		Entity* spinning_text = createEntity();
		ComponentHandle<TextComponent> spinning_text_handle = spinning_text->addComponentByClass<TextComponent>();
		TextComponent& spinning_text_comp = ECS::GetComponent(spinning_text_handle);
		spinning_text_comp.setTextDatas("Benchmark", AssetManager::GetFont("arial_24"));
		spinning_text_comp.position = HudPosition{ Vector2{ 0.0f, 1.0f }, Vector2::halfUnit, Vector2{ 130.0f + x * 180.0f, -950.0f } };
		spinning_text_comp.scale = Vector2{ 1.2f };
		spinning_texts.push_back(spinning_text_handle);
	}
}

void BenchmarkRendering2D::unloadScene()
{
}

void BenchmarkRendering2D::updateScene(float dt)
{
	for (auto& spinning_text_handle : spinning_texts)
	{
		TextComponent& spinning_text_comp = ECS::GetComponent(spinning_text_handle);
		spinning_text_comp.rotAngle = Maths::fmod(spinning_text_comp.rotAngle + dt * 180.0f, 360.0f);
	}
}
