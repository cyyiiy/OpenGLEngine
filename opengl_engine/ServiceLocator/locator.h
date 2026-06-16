#pragma once
#include "renderer.h"
#include "physics.h"
#include "audio.h"
#include "log.h"
#include <memory>


class Locator
{
public:
	static Renderer& getRenderer();
	static Renderer& provideRenderer(std::weak_ptr<Renderer> rendererService_);

	static Physics& getPhysics();
	static Physics& providePhysics(std::weak_ptr<Physics> physicsService_);

	static Audio& getAudio();
	static Audio& provideAudio(std::weak_ptr<Audio> audioService_);

	static Log& getLog();
	static Log& provideLog(std::weak_ptr<Log> logService_);

private:
	static std::shared_ptr<Renderer> rendererService;
	static std::shared_ptr<Physics> physicsService;
	static std::shared_ptr<Audio> audioService;
	static std::shared_ptr<Log> logService;

	friend class Engine;
	static void initialize();
};

