#pragma once
#include <stdint.h>
#include <functional>

namespace FMOD
{
	class Sound;
}

// Unsigned char cause we don't need more than 8 bits
typedef unsigned char SoundSettings;
#define ACTIVATE_3D		1 << 0	// Make this sound a 3D sound, capable of being played at a certain position in the world
#define ACTIVATE_STREAM	1 << 1	// Make the sound streaming from its file at runtime (use on big sound files)
#define ACTIVATE_LOOP	1 << 2	// Enable loop playing on this sound


enum class ChannelSpatialization : uint8_t
{
	Channel2D = 0,	// For a channel that can play 2D sound
	Channel3D = 1	// For a channel that can play 3D sound (movable in space)
};

struct AudioSoundPtr
{
	FMOD::Sound* sound;
};


struct AudioCollisionOcclusion
{
	AudioCollisionOcclusion() : directOcclusion(1.0f), reverbOcclusion(0.5f) {}
	AudioCollisionOcclusion(float directOcclusion_, float reverbOcclusion_) : directOcclusion(directOcclusion_), reverbOcclusion(reverbOcclusion_) {}

	float directOcclusion{ 1.0f }; // Occlusion factor of the polygon for the direct path where 0 represents no occlusion and 1 represents full occlusion.
	float reverbOcclusion{ 0.5f }; // Occlusion factor of the polygon for the reverb path where 0 represents no occlusion and 1 represents full occlusion.


	bool operator==(const AudioCollisionOcclusion& other) const
	{
		return (directOcclusion == other.directOcclusion && reverbOcclusion == other.reverbOcclusion);
	}
};

template<>
struct std::hash<AudioCollisionOcclusion>
{
	std::size_t operator()(const AudioCollisionOcclusion& aco) const noexcept
	{
		return std::hash<float>{}(aco.directOcclusion) ^ (std::hash<float>{}(aco.reverbOcclusion) << 1);
	}
};
// Note: The hash function for this type is not needed (it would be if it was the key of a map or if it was used in a set for exemple), but I let it here so it can be used as an example.