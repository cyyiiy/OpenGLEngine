#include "gameplayStatics.h"
#include <ServiceLocator/locator.h>
#include <ECS/ecs.h>
#include <Rendering/shapeRendererComponent.h>
#include <Rendering/Shapes/shapePoint.h>
#include <Rendering/Shapes/shapeLine.h>
#include <Rendering/Shapes/shapeCube.h>

Game* GameplayStatics::currentGame = nullptr;
Scene* GameplayStatics::currentScene = nullptr;
Vector2Int GameplayStatics::windowSize = Vector2Int::zero;
float GameplayStatics::deltaTime = 0.0f;
float GameplayStatics::engineTime = 0.0f;
Event<const Vector2Int> GameplayStatics::OnScrenResize;


Game* GameplayStatics::GetGame()
{
	if (!currentGame)
	{
		Locator::getLog().LogMessage_Category("Gameplay Statics: There is no currently referenced Game.", LogCategory::Error);
		return nullptr;
	}

	return currentGame;
}

Scene* GameplayStatics::GetActiveScene()
{
	if (!currentScene)
	{
		Locator::getLog().LogMessage_Category("Gameplay Statics: There is no active Scene.", LogCategory::Error);
		return nullptr;
	}

	return currentScene;
}

Vector2Int GameplayStatics::GetWindowSize()
{
	return windowSize;
}

float GameplayStatics::GetDeltaTime()
{
	return deltaTime;
}

float GameplayStatics::GetEngineTime()
{
	return engineTime;
}

void GameplayStatics::DrawDebugPoint(const Vector3& pointPosition, const Color& color, float duration)
{
	ShapeRendererComponent& shape_renderer_component = ECS::GetComponent(ECS::CreateComponent<ShapeRendererComponent>());
	shape_renderer_component.shape = std::make_unique<ShapePoint>(pointPosition, color);
	shape_renderer_component.lifetime = duration;
}

void GameplayStatics::DrawDebugLine(const Vector3& pointA, const Vector3& pointB, const Color& color, float duration)
{
	ShapeRendererComponent& shape_renderer_component = ECS::GetComponent(ECS::CreateComponent<ShapeRendererComponent>());
	shape_renderer_component.shape = std::make_unique<ShapeLine>(pointA, pointB, color);
	shape_renderer_component.lifetime = duration;
}

void GameplayStatics::DrawDebugCube(const Box& boxInfos, const Color& color, float duration)
{
	ShapeRendererComponent& shape_renderer_component = ECS::GetComponent(ECS::CreateComponent<ShapeRendererComponent>());
	shape_renderer_component.shape = std::make_unique<ShapeCube>(boxInfos, color);
	shape_renderer_component.lifetime = duration;
}


void GameplayStatics::SetCurrentGame(Game* game)
{
	currentGame = game;
}

void GameplayStatics::SetCurrentScene(Scene* scene)
{
	currentScene = scene;
}

void GameplayStatics::SetWindowSize(const Vector2Int& size)
{
	windowSize = size;
}

void GameplayStatics::SetDeltaTime(float time)
{
	deltaTime = time;
}

void GameplayStatics::SetEngineTime(float time)
{
	engineTime = time;
}

void GameplayStatics::UpdateDebugs(float deltaTime)
{
	auto& shape_renderers_manager = ECS::Manager<ShapeRendererComponent>();
	shape_renderers_manager.ForEach([deltaTime](ShapeRendererComponent& shape_renderer_component)
	{
		shape_renderer_component.lifetime -= deltaTime;
		if (shape_renderer_component.lifetime <= 0.0f)
		{
			ECS::DeleteComponent(shape_renderer_component.getSelfHandle<ShapeRendererComponent>());
		}
	});
}
