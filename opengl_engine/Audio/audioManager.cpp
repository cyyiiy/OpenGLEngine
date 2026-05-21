#include "audioManager.h"
#include "Utils/defines.h"
#include <ServiceLocator/locator.h>
#include <iostream>


AudioManager::~AudioManager()
{
	Locator::provideAudio(nullptr);
}


// --------------------------------------------------------------
//            Core part
// --------------------------------------------------------------

// Initialize and Quit
bool AudioManager::Initialize(const float maxWorldSize)
{
	FMOD_RESULT result;

	// 1. Intialize fmod debug
	result = FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_TTY);
	if (result != FMOD_OK)
	{
		std::cout << std::endl;
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to initialize FMod debug." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	// 2. Create fmod core system object
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		std::cout << std::endl;
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to create FMOD core system." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	// 3. Initialize fmod core system
	result = system->init(MAX_CHANNELS, FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
	{
		std::cout << std::endl;
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to initialize FMOD core system." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	// 4. Setup basic 3D settings
	result = system->set3DSettings(1.0f, 1.0f, 1.0f);
	if (result != FMOD_OK)
	{
		std::cout << std::endl;
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to setup FMOD 3D settings." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	// 5. Setup geometry settings
	result = system->setGeometrySettings(maxWorldSize);
	if (result != FMOD_OK)
	{
		std::cout << std::endl;
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to setup FMOD Geometry settings." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	return true;
}

void AudioManager::Quit()
{
	if (!system) return;
	
	system->release();
	system = nullptr;
}


// Updates
void AudioManager::Update()
{
	if (!system) return;

	system->update();
}

void AudioManager::UpdateListener(const Vector3 listenerPos, const Vector3 listenerUp, const Vector3 listenerForward)
{
	if (!system) return;

	const FMOD_VECTOR listener_pos = listenerPos.toFMOD();
	const FMOD_VECTOR listener_vel = Vector3::zero.toFMOD();
	const FMOD_VECTOR listener_forward = (-listenerForward).toFMOD(); // Note: need to reverse forward otherwise the sound come from the wrong ear (don't know why)
	const FMOD_VECTOR listener_up = listenerUp.toFMOD();

	FMOD_RESULT result;
	result = system->set3DListenerAttributes(0, &listener_pos, &listener_vel, &listener_forward, &listener_up);

	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to update FMod Listener." + ErrorFModString(result), LogCategory::Error);
	}
}



// --------------------------------------------------------------
//            Engine pause part
// --------------------------------------------------------------

void AudioManager::PauseAll()
{
	if (!system) return;

	FMOD::ChannelGroup* master;
	system->getMasterChannelGroup(&master);

	FMOD_RESULT result;
	result = master->setPaused(true);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to pause all sounds." + ErrorFModString(result), LogCategory::Error);
	}
}

void AudioManager::ResumeAll()
{
	if (!system) return;

	FMOD::ChannelGroup* master;
	system->getMasterChannelGroup(&master);

	FMOD_RESULT result;
	result = master->setPaused(false);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to resume all sounds." + ErrorFModString(result), LogCategory::Error);
	}
}



// --------------------------------------------------------------
//            Is Valid
// --------------------------------------------------------------

bool AudioManager::IsAudioSystemValid()
{
	return system != nullptr;
}



// --------------------------------------------------------------
//            Load Sound part
// --------------------------------------------------------------

AudioSoundPtr AudioManager::LoadSound(std::string soundFile, const SoundSettings settings)
{
	const std::string sound_path = RESOURCES_PATH + soundFile;

	// 3. Convert SoundSettings into FMOD settings
	FMOD_MODE spatialization_mode = FMOD_2D;
	if (settings & ACTIVATE_3D)
		spatialization_mode = FMOD_3D;

	FMOD_MODE load_mode = FMOD_CREATESAMPLE;
	if (settings & ACTIVATE_STREAM)
		load_mode = FMOD_CREATESTREAM;

	FMOD_MODE loop_mode = FMOD_LOOP_OFF;
	if (settings & ACTIVATE_LOOP)
		loop_mode = FMOD_LOOP_NORMAL;


	// 2. Load the sound from file with chosen settings
	FMOD::Sound* sound;
	FMOD_RESULT result;
	result = system->createSound(sound_path.c_str(), spatialization_mode | load_mode | loop_mode | FMOD_3D_LINEARSQUAREROLLOFF, 0, &sound);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to load a sound from file." + ErrorFModString(result), LogCategory::Error);
	}

	return AudioSoundPtr{ sound }; // Wrap fmod sound in a struct to avoid other class having to know about fmod
}



// --------------------------------------------------------------
//            Global Settings part
// --------------------------------------------------------------

void AudioManager::SetGlobalVolume(const float volume)
{
	if (!system) return;

	FMOD::ChannelGroup* master;
	system->getMasterChannelGroup(&master);

	FMOD_RESULT result;
	result = master->setVolume(volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set global volume." + ErrorFModString(result), LogCategory::Error);
	}
}

float AudioManager::GetGlobalVolume()
{
	if (!system) return 0.0f;

	FMOD::ChannelGroup* master;
	system->getMasterChannelGroup(&master);

	float volume;
	FMOD_RESULT result;
	result = master->getVolume(&volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to get global volume." + ErrorFModString(result), LogCategory::Error);
		return 0.0f;
	}

	return volume;
}



// --------------------------------------------------------------
//            Audio Source part
// --------------------------------------------------------------

// Create and Release
std::uint32_t AudioManager::CreateAudioSourceGroup(const ChannelSpatialization spatialization)
{
	if (!system) return 0;

	FMOD_RESULT result;

	audioSourcesGroups.emplace(audioSourcesGroupsID, nullptr); // Reserve the memory in the map
	result = system->createChannelGroup("", &audioSourcesGroups[audioSourcesGroupsID]); // Actually create the ChannelGroup object
	audioSourcesGroupsID++; // Increment the total channel group count (even if it failed)

	if (result != FMOD_OK) // Check if the ChannelGroup was correctly created
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to create a new audio source group." + ErrorFModString(result), LogCategory::Error);
		return audioSourcesGroupsID - 1;
	}

	if (spatialization == ChannelSpatialization::Channel3D)
	{
		FMOD::ChannelGroup* group_create = audioSourcesGroups[audioSourcesGroupsID - 1];
		group_create->setMode(FMOD_3D);
	}

	return audioSourcesGroupsID - 1; // Return the index of the created channel group
}

void AudioManager::ReleaseAudioSourceGroup(const std::uint32_t index)
{
	// 1. Retrieve the group to release
	FMOD::ChannelGroup* group_release = audioSourcesGroups[index];
	if (group_release == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to release an audio source group with a non-registered index.", LogCategory::Error);
		return;
	}

	// 2. Release the group (fmod)
	FMOD_RESULT result;
	result = group_release->release();
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to release an audio source group." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	audioSourcesGroups.erase(index); // Free the unused memory in the map
}


// Play and Stop
void AudioManager::PlaySoundOnAudioSource(const std::uint32_t index, const AudioSound& sound, const int loop)
{
	if (!system) return;

	if (!sound.isValid()) // Check if the AudioSound object has a valid FMOD sound
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to play a sound on an audio source group with an unitialized sound.", LogCategory::Error);
		return;
	}

	FMOD::ChannelGroup* group_sound = audioSourcesGroups[index]; // Get the ChannelGroup of given index and check if it is valid
	if (group_sound == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to play a sound on an audio source group with a non-registered index.", LogCategory::Error);
		return;
	}

	if (sound.hasSetting(ACTIVATE_3D) != (GetGroupSpatialization(group_sound) == ChannelSpatialization::Channel3D)) // Check if the sound and audio source has the same spatialization
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to play a sound on an audio source that hasn't the same spatialization.", LogCategory::Warning);
		return;
	}


	group_sound->stop(); // Stop potential sound already playing on the ChannelGroup (only one sound at a time on a single Audio Source)

	// Play the sound and retrieve the channel it is played on (check if the sound has correctly been played)
	FMOD::Channel* channel;
	FMOD_RESULT result;
	result = system->playSound(sound.getFMod(), group_sound, false, &channel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to play a sound on an audio source group." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	// Loop the sound (if the sound can be looped)
	if (loop != 0 && !sound.hasSetting(ACTIVATE_LOOP))
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to play a sound on an audio source group with loop on an sound that hasn't loop activated.", LogCategory::Warning);
	}
	else
	{
		result = channel->setLoopCount(loop);
		if (result != FMOD_OK)
		{
			Locator::getLog().LogMessage_Category("Audio Manager: Failed to set loop count on an sound played on an audio source group." + ErrorFModString(result), LogCategory::Error);
			// Don't return if setting loop failed, the sound has already started playing so we need to setup the position of the channel
		}
	}

	// Position the channel in space if the sound is in 3D
	if (sound.hasSetting(ACTIVATE_3D))
	{
		FMOD_VECTOR pos;
		FMOD_VECTOR vel;
		group_sound->get3DAttributes(&pos, &vel);
		channel->set3DAttributes(&pos, &vel);
	}
}

void AudioManager::StopAudioSource(const std::uint32_t index)
{
	FMOD::ChannelGroup* group_stop = audioSourcesGroups[index];
	if (group_stop == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to stop an audio source group with a non-registered index.", LogCategory::Error);
		return;
	}

	FMOD_RESULT result;
	result = group_stop->stop();
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to stop an audio source group." + ErrorFModString(result), LogCategory::Error);
	}
}


// Pause (set and get)
void AudioManager::PauseAudioSource(const std::uint32_t index, const bool pause)
{
	FMOD::ChannelGroup* group_pause = audioSourcesGroups[index];
	if (group_pause == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to pause an audio source group with a non-registered index.", LogCategory::Error);
		return;
	}

	FMOD_RESULT result;
	result = group_pause->setPaused(pause);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to pause an audio source group." + ErrorFModString(result), LogCategory::Error);
	}
}

bool AudioManager::GetAudioSourcePaused(const std::uint32_t index)
{
	FMOD::ChannelGroup* group_pause = audioSourcesGroups[index];
	if (group_pause == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to get an audio source group paused with a non-registered index.", LogCategory::Error);
		return false;
	}

	bool paused = false;
	FMOD_RESULT result;
	result = group_pause->getPaused(&paused);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to get an audio source group paused." + ErrorFModString(result), LogCategory::Error);
		return false;
	}

	return paused;
}


// Spatialization (set and get)
void AudioManager::SetAudioSourceSpatialization(const std::uint32_t index, const ChannelSpatialization spatialization)
{
	FMOD::ChannelGroup* group_spatialization = audioSourcesGroups[index];
	if (group_spatialization == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to set an audio source group spatialization with a non-registered index.", LogCategory::Error);
		return;
	}

	FMOD_RESULT result;
	switch (spatialization)
	{
	case ChannelSpatialization::Channel2D:
		result = group_spatialization->setMode(FMOD_2D);
		break;

	case ChannelSpatialization::Channel3D:
		result = group_spatialization->setMode(FMOD_3D);
		break;

	default:
		result = FMOD_OK;
		break;
	}
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set an audio source group spatialization." + ErrorFModString(result), LogCategory::Error);
	}
}

ChannelSpatialization AudioManager::GetAudioSourceSpatialization(const std::uint32_t index)
{
	FMOD::ChannelGroup* group_spatialization = audioSourcesGroups[index];
	if (group_spatialization == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to get an audio source group spatialization with a non-registered index.", LogCategory::Error);
		return ChannelSpatialization::Channel3D;
	}

	return GetGroupSpatialization(group_spatialization);
}


// Volume (set and get)
void AudioManager::SetAudioSourceGroupVolume(const std::uint32_t index, const float volume)
{
	FMOD::ChannelGroup* group_volume = audioSourcesGroups[index];
	if (group_volume == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to set an audio source group volume with a non-registered index.", LogCategory::Error);
		return;
	}

	FMOD_RESULT result;
	result = group_volume->setVolume(volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set an audio source group volume." + ErrorFModString(result), LogCategory::Error);
	}
}

float AudioManager::GetAudioSourceGroupVolume(const std::uint32_t index)
{
	FMOD::ChannelGroup* group_volume = audioSourcesGroups[index];
	if (group_volume == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to get an audio source group volume with a non-registered index.", LogCategory::Error);
		return 0.0f;
	}

	float volume;
	FMOD_RESULT result;
	result = group_volume->getVolume(&volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to get an audio source group volume." + ErrorFModString(result), LogCategory::Error);
		return 0.0f;
	}

	return volume;
}


// Position (set and get)
void AudioManager::SetAudioSourceGroupPos(const std::uint32_t index, const Vector3 position)
{
	FMOD::ChannelGroup* group_pos = audioSourcesGroups[index];
	if (group_pos == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to set an audio source group position with a non-registered index.", LogCategory::Error);
		return;
	}

	if (GetGroupSpatialization(group_pos) != ChannelSpatialization::Channel3D)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to set an audio source group position on a 2D audio source.", LogCategory::Warning);
		return;
	}

	const FMOD_VECTOR pos = position.toFMOD();
	const FMOD_VECTOR vel = Vector3::zero.toFMOD();

	FMOD_RESULT result;
	result = group_pos->set3DAttributes(&pos, &vel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set an audio source group position." + ErrorFModString(result), LogCategory::Error);
	}

	int current_nb_channel_playing;
	group_pos->getNumChannels(&current_nb_channel_playing);
	if (current_nb_channel_playing <= 0) return;

	FMOD::Channel* channelPlaying;
	result = group_pos->getChannel(0, &channelPlaying);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to retrieve the playing channel of an audio source group." + ErrorFModString(result), LogCategory::Error);
	}

	result = channelPlaying->set3DAttributes(&pos, &vel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set the position of the playing channel of an audio source group." + ErrorFModString(result), LogCategory::Error);
	}
}

Vector3 AudioManager::GetAudioSourceGroupPos(const std::uint32_t index)
{
	FMOD::ChannelGroup* group_pos = audioSourcesGroups[index];
	if (group_pos == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to get an audio source group position with a non-registered index.", LogCategory::Error);
		return Vector3::zero;
	}

	if (GetGroupSpatialization(group_pos) != ChannelSpatialization::Channel3D)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to get an audio source group position on a 2D audio source.", LogCategory::Warning);
		return Vector3::zero;
	}

	FMOD_VECTOR pos;
	FMOD_VECTOR vel;

	FMOD_RESULT result;
	result = group_pos->get3DAttributes(&pos, &vel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to get an audio source group position." + ErrorFModString(result), LogCategory::Error);
		return Vector3::zero;
	}

	return Vector3::FromFMOD(pos);
}



// --------------------------------------------------------------
//            Instant Play Sound part
// --------------------------------------------------------------

void AudioManager::InstantPlaySound2D(const AudioSound& sound, const float volume, const int loop)
{
	if (!system) return;

	// 0. Check the sound validity and settings 
	if (!sound.isValid()) // Check if the AudioSound has a valid FMOD sound
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 2D sound with an unitialized sound.", LogCategory::Error);
		return;
	}

	if (sound.hasSetting(ACTIVATE_3D)) // Check if the sound is in 2D
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 2D sound with a 3D sound.", LogCategory::Warning);
		return;
	}

	// 1. Play the sound and retrieve the channel it is played on
	FMOD::Channel* channel;
	FMOD_RESULT result;
	result = system->playSound(sound.getFMod(), 0, false, &channel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to instantly play a 2D sound." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	// 2. Loop the sound (if the sound can be looped)
	if (loop != 0 && !sound.hasSetting(ACTIVATE_LOOP))
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 2D sound with loop on an sound that hasn't loop activated.", LogCategory::Warning);
	}
	else
	{
		result = channel->setLoopCount(loop);
		if (result != FMOD_OK)
		{
			Locator::getLog().LogMessage_Category("Audio Manager: Failed to set loop count on an instantly played 2D sound." + ErrorFModString(result), LogCategory::Error);
			// Don't return if setting loop failed, the sound has already started playing so we need to setup the volume of the channel
		}
	}

	// 3. Set the volume on the created channel
	result = channel->setVolume(volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set volume on an instantly played 2D sound." + ErrorFModString(result), LogCategory::Error);
	}
}

void AudioManager::InstantPlaySound3D(const AudioSound& sound, const Vector3 playPosition, const float volume, const int loop)
{
	if (!system) return;

	// 0. Check the sound validity and settings 
	if (!sound.isValid()) // Check if the AudioSound has a valid FMOD sound
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 3D sound with an unitialized sound.", LogCategory::Error);
		return;
	}

	if (!sound.hasSetting(ACTIVATE_3D)) // Check if the sound is in 3D
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 3D sound with a 2D sound.", LogCategory::Warning);
		return;
	}

	// 1. Play the sound and retrieve the channel it is played on
	FMOD::Channel* channel;
	FMOD_RESULT result;
	result = system->playSound(sound.getFMod(), 0, false, &channel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to instantly play a 3D sound." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	// 2. Loop the sound (if the sound can be looped)
	if (loop != 0 && !sound.hasSetting(ACTIVATE_LOOP))
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to instantly play a 3D sound with loop on an sound that hasn't loop activated.", LogCategory::Warning);
	}
	else
	{
		result = channel->setLoopCount(loop);
		if (result != FMOD_OK)
		{
			Locator::getLog().LogMessage_Category("Audio Manager: Failed to set loop count on an instantly played 3D sound." + ErrorFModString(result), LogCategory::Error);
			// Don't return if setting loop failed, the sound has already started playing so we need to setup the position and volume of the channel
		}
	}

	// 3. Set the volume on the created channel
	result = channel->setVolume(volume);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set volume on an instantly played 3D sound." + ErrorFModString(result), LogCategory::Error);
		// Don't return if setting volume failed, the sound has already started playing so we need to setup the position of the channel
	}

	// 4. Set the position on the created channel
	FMOD_VECTOR sound_pos = playPosition.toFMOD();
	FMOD_VECTOR sound_vel = Vector3::zero.toFMOD();
	result = channel->set3DAttributes(&sound_pos, &sound_vel);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set position on an instantly played 3D sound." + ErrorFModString(result), LogCategory::Error);
	}
}



// --------------------------------------------------------------
//            Geometry (Collisions) part
// --------------------------------------------------------------

std::uint32_t AudioManager::CreateCollision(const int maxPolygons, const int maxVertices)
{
	if (!system) return 0;

	return 0; // Geometry disabled until I achieve to make it work

	FMOD_RESULT result;

	collisions.emplace(collisionsID, nullptr); // Reserve the memory in the map
	result = system->createGeometry(maxPolygons, maxVertices, &collisions[collisionsID]); // Actually create the Geometry object
	collisionsID++; // Increment the total collision count (even if it failed)

	if (result != FMOD_OK) // Check if the Geometry was correctly created
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to create a new collision." + ErrorFModString(result), LogCategory::Error);
		return collisionsID - 1;
	}

	return collisionsID - 1; // Return the index of the created collision
}

void AudioManager::ReleaseCollision(const std::uint32_t index)
{
	return; // Geometry disabled until I achieve to make it work

	// Retrieve the collision to release
	FMOD::Geometry* collision_release = collisions[index];
	if (collision_release == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to release a collision with a non-registered index.", LogCategory::Error);
		return;
	}

	// Release the collision (fmod)
	FMOD_RESULT result;
	result = collision_release->release();
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to release a collision." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	collisions.erase(index); // Free the unused memory in the map
}

void AudioManager::AddPolygonToCollision(const std::uint32_t index, const AudioCollisionOcclusion& audioCollisionType, const bool doubleSided, const std::vector<Vector3> vertices)
{
	return; // Geometry disabled until I achieve to make it work

	// Retrieve the collision to modify
	FMOD::Geometry* collision_polygon = collisions[index];
	if (collision_polygon == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to add a polygon to a collision with a non-registered index.", LogCategory::Error);
		return;
	}

	// Converts Vector3 to FMOD_VECTOR
	std::vector<FMOD_VECTOR> vertices_fmod;
	vertices_fmod.reserve(vertices.size());
	for (Vector3 vertex : vertices)
	{
		vertices_fmod.push_back(vertex.toFMOD());
	}

	// Add the polygon
	FMOD_RESULT result;
	result = collision_polygon->addPolygon(audioCollisionType.directOcclusion, audioCollisionType.reverbOcclusion, doubleSided, static_cast<int>(vertices.size()), &vertices_fmod[0], 0);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to add a polygon to a collision." + ErrorFModString(result), LogCategory::Error);
		return;
	}
}

void AudioManager::SetCollisionTransform(const std::uint32_t index, const Transform& transform)
{
	return; // Geometry disabled until I achieve to make it work

	// Retrieve the collision to modify
	FMOD::Geometry* collision_transform = collisions[index];
	if (collision_transform == nullptr)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Tried to set the transform of a collision with a non-registered index.", LogCategory::Error);
		return;
	}

	FMOD_RESULT result;
	const FMOD_VECTOR position = transform.getPosition().toFMOD();
	const FMOD_VECTOR scale = transform.getScale().toFMOD();

	// Set position
	result = collision_transform->setPosition(&position);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set a collision position." + ErrorFModString(result), LogCategory::Error);
		return;
	}

	// Set scale
	result = collision_transform->setScale(&scale);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to set a collision scale." + ErrorFModString(result), LogCategory::Error);
		return;
	}
}



// --------------------------------------------------------------
//            Helping Functions part
// --------------------------------------------------------------

ChannelSpatialization AudioManager::GetGroupSpatialization(FMOD::ChannelGroup* group)
{
	FMOD_RESULT result;
	FMOD_MODE mode;

	result = group->getMode(&mode);
	if (result != FMOD_OK)
	{
		Locator::getLog().LogMessage_Category("Audio Manager: Failed to get the mode of a channel group." + ErrorFModString(result), LogCategory::Error);
		return ChannelSpatialization::Channel2D;
	}

	if (mode & FMOD_2D)
	{
		return ChannelSpatialization::Channel2D;
	}

	if (mode & FMOD_3D)
	{
		return ChannelSpatialization::Channel3D;
	}

	return ChannelSpatialization::Channel3D;
}

std::string AudioManager::ErrorFModString(FMOD_RESULT fmodError)
{
	return " | Associated FMod Error: " + std::string(FMOD_ErrorString(fmodError));
}
