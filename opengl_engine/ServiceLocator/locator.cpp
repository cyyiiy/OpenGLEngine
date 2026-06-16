#include "locator.h"
#include "nullRenderer.h"
#include "nullPhysics.h"
#include "nullAudio.h"
#include "nullLog.h"

std::shared_ptr<Renderer> Locator::rendererService;
std::shared_ptr<Physics> Locator::physicsService;
std::shared_ptr<Audio> Locator::audioService;
std::shared_ptr<Log> Locator::logService;


Renderer& Locator::getRenderer()
{
	return *rendererService;
}

Renderer& Locator::provideRenderer(std::weak_ptr<Renderer> rendererService_)
{
	if (rendererService_.expired())
	{
		rendererService = std::make_shared<NullRenderer>();
		return getRenderer();
	}
	else
	{
		rendererService = rendererService_.lock();
		return getRenderer();
	}
}

Physics& Locator::getPhysics()
{
	return *physicsService;
}

Physics& Locator::providePhysics(std::weak_ptr<Physics> physicsService_)
{
	if (physicsService_.expired())
	{
		physicsService = std::make_shared<NullPhysics>();
		return getPhysics();
	}
	else
	{
		physicsService = physicsService_.lock();
		return getPhysics();
	}
}

Audio& Locator::getAudio()
{
	return *audioService;
}

Audio& Locator::provideAudio(std::weak_ptr<Audio> audioService_)
{
	if (audioService_.expired())
	{
		audioService = std::make_shared<NullAudio>();
		return getAudio();
	}
	else
	{
		audioService = audioService_.lock();
		return getAudio();
	}
}

Log& Locator::getLog()
{
	return *logService;
}

Log& Locator::provideLog(std::weak_ptr<Log> logService_)
{
	if (logService_.expired())
	{
		logService = std::make_shared<NullLog>();
		return getLog();
	}
	else
	{
		logService = logService_.lock();
		return getLog();
	}
}

void Locator::initialize()
{
	rendererService = std::make_shared<NullRenderer>();
	physicsService = std::make_shared<NullPhysics>();
	audioService = std::make_shared<NullAudio>();
	logService = std::make_shared<NullLog>();
}
