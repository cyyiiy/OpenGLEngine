#pragma once
#include <Assets/assetInterface.h>
#include <Assets/cyassetDocument.h>
#include "audioUtils.h"


class AudioSound : public IAsset
{
public:
	// Asset part
	struct LoadParams
	{
		std::filesystem::path soundFilePath;
		SoundSettings soundSettings;
		float fadingMinDistance;
		float fadingMaxDistance;
	};

	AudioSound(AudioSoundPtr _soundPtr, uint64_t _soundPtrMemory, SoundSettings _soundSettings);
	~AudioSound();

	AudioSound(const AudioSound&) = delete;
	AudioSound(AudioSound&&) = delete;
	AudioSound& operator=(const AudioSound&) = delete;
	AudioSound& operator=(AudioSound&&) = delete;


	static std::string GetTypeName();
	static std::shared_ptr<AudioSound> Create(const LoadParams& params);
	static LoadParams ParseCyasset(const CyassetDocument& cyasset);

	[[nodiscard]] uint64_t getAssetMemorySize() const override;
	[[nodiscard]] uint64_t getAssetGpuSize() const override;


	// Audio Sound part
	bool isValid() const;
	AudioSoundPtr getSoundPtr() const;
	bool hasSetting(SoundSettings setting) const;

private:
	AudioSoundPtr soundPtr;
	uint64_t soundPtrMemory;
	SoundSettings soundSettings;
};