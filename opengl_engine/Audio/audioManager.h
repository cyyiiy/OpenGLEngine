#pragma once
#include <ServiceLocator/audio.h>

#include <FMod/fmod.hpp>
#include <FMod/fmod_errors.h>

#include "audioUtils.h"
#include "audioSound.h"

#include <Maths/Vector3.h>
#include <Core/transform.h>

#include <string>
#include <unordered_map>


const int MAX_CHANNELS = 512;

/**
* The audio service provider class.
*/
class AudioManager : public Audio
{
public:
	// ----------------------------------------------------------
	//  Global (available from audio interface)
	// ----------------------------------------------------------
	bool IsAudioSystemValid() override;

	// ----------------------------------------------------------
	//  Load Sound (available from audio interface)
	// ----------------------------------------------------------
	AudioSoundPtr LoadSound(std::string soundFile, const SoundSettings settings) override;


	// ----------------------------------------------------------
	//  Global settings (available from audio interface)
	// ----------------------------------------------------------
	void SetGlobalVolume(const float volume) override;
	float GetGlobalVolume() override;


	// ----------------------------------------------------------
	//  Audio Source (available from audio interface)
	// ----------------------------------------------------------
	std::uint32_t CreateAudioSourceGroup(const ChannelSpatialization spatialization) override;
	void ReleaseAudioSourceGroup(const std::uint32_t index) override;

	void PlaySoundOnAudioSource(const std::uint32_t index, const AudioSound& sound, const int loop) override;
 	void StopAudioSource(const std::uint32_t index) override;

	void PauseAudioSource(const std::uint32_t index, const bool pause) override;
	bool GetAudioSourcePaused(const std::uint32_t index) override;

	void SetAudioSourceSpatialization(const std::uint32_t index, const ChannelSpatialization spatialization) override;
	ChannelSpatialization GetAudioSourceSpatialization(const std::uint32_t index) override;

	void SetAudioSourceGroupVolume(const std::uint32_t index, const float volume) override;
	float GetAudioSourceGroupVolume(const std::uint32_t index) override;

	void SetAudioSourceGroupPos(const std::uint32_t index, const Vector3 position) override;
	Vector3 GetAudioSourceGroupPos(const std::uint32_t index) override;


	// ----------------------------------------------------------
	//  Instant Play Sound (available from audio interface)
	// ----------------------------------------------------------
	void InstantPlaySound2D(const AudioSound& sound, const float volume, const int loop) override;
	void InstantPlaySound3D(const AudioSound& sound, const Vector3 playPosition, const float volume, const int loop) override;



	// ----------------------------------------------------------
	//  Collisions (available from audio interface)
	// ----------------------------------------------------------
	std::uint32_t CreateCollision(const int maxPolygons, const int maxVertices) override;
	void ReleaseCollision(const std::uint32_t index) override;

	void AddPolygonToCollision(const std::uint32_t index, const AudioCollisionOcclusion& audioCollisionType, const bool doubleSided, const std::vector<Vector3> vertices) override;
	void SetCollisionTransform(const std::uint32_t index, const Transform& transform) override;




	// ----------------------------------------------------------
	//  Audio Management
	// ----------------------------------------------------------
	bool Initialize(const float maxWorldSize) override;
	void Quit() override;

	void Update() override;
	void UpdateListener(const Vector3 listenerPos, const Vector3 listenerUp, const Vector3 listenerForward) override;

	void PauseAll() override;
	void ResumeAll() override;


private:
	FMOD::System* system{ nullptr };

	std::unordered_map<std::uint32_t, FMOD::ChannelGroup*> audioSourcesGroups;
	std::uint32_t audioSourcesGroupsID{ 0 };

	std::unordered_map<std::uint32_t, FMOD::Geometry*> collisions;
	std::uint32_t collisionsID{ 0 };


	// ----------------------------------------------------------
	//  Helping Functions (private)
	// ----------------------------------------------------------
	ChannelSpatialization GetGroupSpatialization(FMOD::ChannelGroup* group);
	std::string ErrorFModString(FMOD_RESULT fmodError);
};

