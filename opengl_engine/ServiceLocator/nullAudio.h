#pragma once
#include <ServiceLocator/audio.h>
#include <Audio/audioSound.h>

/**
* The audio null service provider class.
*/
class NullAudio : public Audio
{
public:
	bool IsAudioSystemValid() override { return false; }


	AudioSoundPtr LoadSound(std::string soundFile, const SoundSettings settings = 0) override { return { nullptr }; }


	void SetGlobalVolume(const float volume) override {}
	float GetGlobalVolume() override { return 0.0f; }


	std::uint32_t CreateAudioSourceGroup(ChannelSpatialization spatialization) override { return 0; }
	void ReleaseAudioSourceGroup(const std::uint32_t index) override {}

	void PlaySoundOnAudioSource(const std::uint32_t index, const AudioSound& sound, const int loop = 0) override {}
	void StopAudioSource(const std::uint32_t index) override {}

	void PauseAudioSource(const std::uint32_t index, const bool pause) override {}
	bool GetAudioSourcePaused(const std::uint32_t index) override { return false; }

	void SetAudioSourceSpatialization(const std::uint32_t index, const ChannelSpatialization spatialization) override {}
	ChannelSpatialization GetAudioSourceSpatialization(const std::uint32_t index) override { return ChannelSpatialization::Channel3D; }

	void SetAudioSourceGroupVolume(const std::uint32_t index, const float volume) override {}
	float GetAudioSourceGroupVolume(const std::uint32_t index) override { return 0.0f; }

	void SetAudioSourceGroupPos(const std::uint32_t index, const Vector3 position) override {}
	Vector3 GetAudioSourceGroupPos(const std::uint32_t index) override { return Vector3::zero; }


	void InstantPlaySound2D(const AudioSound& sound, const float volume = 1.0f, const int loop = 0) override {}
	void InstantPlaySound3D(const AudioSound& sound, const Vector3 playPosition, const float volume = 1.0f, const int loop = 0) override {}


	std::uint32_t CreateCollision(const int maxPolygons, const int maxVertices) override { return 0; }
	void ReleaseCollision(const std::uint32_t index) override {}

	void AddPolygonToCollision(const std::uint32_t index, const AudioCollisionOcclusion& audioCollisionType, const bool doubleSided, const std::vector<Vector3> vertices) override {}
	void SetCollisionTransform(const std::uint32_t index, const Transform& transform) override {}


	bool Initialize(const float maxWorldSize) override { return false; }
	void Quit() override {}

	void Update() override {}
	void UpdateListener(const Vector3 listenerPos, const Vector3 listenerUp, const Vector3 listenerForward) override {}

	void PauseAll() override {}
	void ResumeAll() override {}
};