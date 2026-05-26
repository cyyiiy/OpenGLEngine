#include "locator.h"
#include "nullRenderer.h"
#include "nullPhysics.h"
#include "nullAudio.h"
#include "nullLog.h"

Renderer* Locator::rendererService;
Physics* Locator::physicsService;
Audio* Locator::audioService;
Log* Locator::logService;
NullRenderer Locator::nullRendererService;
NullPhysics Locator::nullPhysicsService;
NullAudio Locator::nullAudioService;
NullLog Locator::nullLogService;


Renderer& Locator::getRenderer()
{
	return *rendererService;
}

Renderer& Locator::provideRenderer(Renderer* rendererService_)
{
	if (rendererService_ == NULL)
	{
		rendererService = &nullRendererService;
		return getRenderer();
	}
	else
	{
		rendererService = rendererService_;
		return getRenderer();
	}
}

Physics& Locator::getPhysics()
{
	return *physicsService;
}

Physics& Locator::providePhysics(Physics* physicsService_)
{
	if (physicsService_ == NULL)
	{
		physicsService = &nullPhysicsService;
		return getPhysics();
	}
	else
	{
		physicsService = physicsService_;
		return getPhysics();
	}
}

Audio& Locator::getAudio()
{
	return *audioService;
}

Audio& Locator::provideAudio(Audio* audioService_)
{
	if (audioService_ == NULL)
	{
		audioService = &nullAudioService;
		return getAudio();
	}
	else
	{
		audioService = audioService_;
		return getAudio();
	}
}

Log& Locator::getLog()
{
	return *logService;
}

Log& Locator::provideLog(Log* logService_)
{
	if (logService_ == NULL)
	{
		logService = &nullLogService;
		return getLog();
	}
	else
	{
		logService = logService_;
		return getLog();
	}
}

void Locator::initialize()
{
	rendererService = &nullRendererService;
	physicsService = &nullPhysicsService;
	audioService = &nullAudioService;
	logService = &nullLogService;
}
