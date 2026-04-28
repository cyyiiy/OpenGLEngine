#pragma once
#include "renderer.h"
//#include "physics.h"
//#include "audio.h"
#include "log.h"

class NullRenderer;
class NullPhysics;
class NullAudio;
class NullLog;

class Locator
{
public:
	static Renderer& getRenderer();
	static Renderer& provideRenderer(Renderer* rendererService_);

	/*
	static Physics& getPhysics();
	static Physics& providePhysics(Physics* physicsService_);

	static Audio& getAudio();
	static Audio& provideAudio(Audio* audioService_);
	*/

	static Log& getLog();
	static Log& provideLog(Log* logService_);

private:
	static Renderer* rendererService;
	static NullRenderer nullRendererService;

	/*
	static Physics* physicsService;
	static NullPhysics nullPhysicsService;

	static Audio* audioService;
	static NullAudio nullAudioService;
	*/

	static Log* logService;
	static NullLog nullLogService;

	friend class Engine;
	static void initialize();
};

