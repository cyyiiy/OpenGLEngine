#pragma once
#include "audioUtils.h"
#include <stdint.h>

namespace FMOD // Avoid including "fmod.hpp"
{
	class Sound;
}

class AudioSound
{
public:
	AudioSound(FMOD::Sound* fmodSound_, const SoundSettings soundSettings);
	~AudioSound();

	AudioSound() = delete;
	AudioSound(const AudioSound& other) = delete;
	AudioSound& operator=(const AudioSound& other) = delete;

	bool isValid() const;
	FMOD::Sound* getFMod() const;
	void releaseFMod();

	bool hasSetting(SoundSettings setting) const;

	// Distances for sound fading
	void setMinMaxDistance(float min, float max) const;

private:
	FMOD::Sound* FModSound{ nullptr };
	SoundSettings Settings{ 0 };
};

