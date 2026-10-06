#pragma once
#include <stdint.h>
#include <functional>

namespace FMOD // Avoid including "fmod.hpp"
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