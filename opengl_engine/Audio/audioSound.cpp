#include "audioSound.h"
#include <ServiceLocator/locator.h>
#include <ServiceLocator/audio.h>
#include <Utils/defines.h>
#include <FMod/fmod.hpp>
#include <FMod/fmod_errors.h>


AudioSound::AudioSound(AudioSoundPtr _soundPtr, uint64_t _soundPtrMemory, SoundSettings _soundSettings) :
	IAsset(), soundPtr(_soundPtr), soundPtrMemory(_soundPtrMemory), soundSettings(_soundSettings)
{}

AudioSound::~AudioSound()
{
	if (!isValid()) return;

	Audio& audio = Locator::getAudio();
	if (!audio.IsAudioSystemValid()) return;

	soundPtr.sound->release();
}


std::string AudioSound::GetTypeName()
{
	return "Audio Sound";
}

std::shared_ptr<AudioSound> AudioSound::Create(const LoadParams& params)
{
	// Convert the sound file path
	std::filesystem::path sound_path = RESOURCES_PATH;
	sound_path += params.soundFilePath;

	// Get the audio manager
	Audio& audio = Locator::getAudio();
	if (!audio.IsAudioSystemValid())
	{
		Locator::getLog().LogMessage_Category("Audio Sound: Couldn't load an audio sound due to the audio manager system being invalid!", LogCategory::Error);
		return nullptr;
	}

	// Get the memory usage of FMod before loading the sound
	int before = 0, max_alloc = 0;
	FMOD::Memory_GetStats(&before, &max_alloc, true);

	// Load the sound
	AudioSoundPtr sound_ptr = audio.LoadSound(sound_path.string(), params.soundSettings);
	if (sound_ptr.sound == nullptr)
	{
		// Note: The audio manager already logged an error
		return nullptr;
	}

	// Get the memory usage of FMod after loading the sound
	int after = 0;
	FMOD::Memory_GetStats(&after, &max_alloc, true);
	const uint64_t sound_memory = uint64_t(after - before);

	// Set the fading min and max distances if relevant
	if (params.soundSettings & ACTIVATE_3D)
	{
		FMOD_RESULT result;
		result = sound_ptr.sound->set3DMinMaxDistance(params.fadingMinDistance, params.fadingMaxDistance);
		if (result != FMOD_OK)
		{
			Locator::getLog().LogMessage_Category("Audio Sound: Failed to set sound fading distances. | Associated FMod Error: " + std::string(FMOD_ErrorString(result)), LogCategory::Error);
		}
	}

	return std::make_shared<AudioSound>(sound_ptr, sound_memory, params.soundSettings);
}

AudioSound::LoadParams AudioSound::ParseCyasset(const CyassetDocument & cyasset)
{
	throw std::exception("Cyasset is not implemented yet.");
}


uint64_t AudioSound::getAssetMemorySize() const
{
	return sizeof(AudioSound) + soundPtrMemory;
}

uint64_t AudioSound::getAssetGpuSize() const
{
	return 0;
}


bool AudioSound::isValid() const
{
	return soundPtr.sound != nullptr;
}

AudioSoundPtr AudioSound::getSoundPtr() const
{
	return soundPtr;
}

bool AudioSound::hasSetting(SoundSettings setting) const
{
	return soundSettings & setting;
}