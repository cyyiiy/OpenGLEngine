#pragma once
#include <Audio/audioUtils.h>
#include <Maths/Vector3.h>
#include <string>
#include <vector>

class AudioSound;
class Transform;
struct AudioCollisionOcclusion;


/**
* The Audio Service class (the virtual class provided by the Locator).
*/
class Audio
{
public:
	virtual ~Audio() {}


// -----------------------------------------------------------------------------
//                 Global
// -----------------------------------------------------------------------------
	/**
	* Check if the Audio System is valid.
	* @return		True if the Audio System is currently valid.
	*/
	virtual bool IsAudioSystemValid() = 0;



// -----------------------------------------------------------------------------
//                 Load Sound
// -----------------------------------------------------------------------------

	/**
	* Load a sound in memory from file.
	* @param	soundFile	The path to the sound file to read.
	* @param	settings	(optionnal) Additionnal load settings for the sound.
	* @return				The newly created Fmod Sound object.
	*/
	virtual AudioSoundPtr LoadSound(std::string soundFile, const SoundSettings settings = 0) = 0;



// -----------------------------------------------------------------------------
//                 Global Volume
// -----------------------------------------------------------------------------

	/**
	* Set the master volume.
	* @param	volume	The volume value to set. Between 0 and 1.
	*/
	virtual void SetGlobalVolume(const float volume) = 0;

	/**
	* Get the master volume value.
	* @return			The volume value.
	*/
	virtual float GetGlobalVolume() = 0;



// -----------------------------------------------------------------------------
//                 Audio Source
// -----------------------------------------------------------------------------

	/**
	* Create a ChannelGroup for an Audio Source component.
	* @param	spatialization	Is the channel made for 2D or 3D.
	* @return					The index of the created ChannelGroup.
	*/
	virtual std::uint32_t CreateAudioSourceGroup(const ChannelSpatialization spatialization) = 0;

	/**
	* Release a ChannelGroup for an Audio Source component destruction.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	*/
	virtual void ReleaseAudioSourceGroup(const std::uint32_t index) = 0;


	/**
	* Play a sound on an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @param	sound		The sound to play. It must've been loaded with the same spatialization than the Audio Source ChannelGroup.
	* @param	loop		The number of time the sound will loop. -1 = infinite loop. 0 = play only once (defaut). The sound must've been loaded with the loop settings.
	*/
	virtual void PlaySoundOnAudioSource(const std::uint32_t index, const AudioSound& sound, const int loop = 0) = 0;

	/**
	* Stop every sound playing on an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	*/
	virtual void StopAudioSource(const std::uint32_t index) = 0;
	

	/**
	* Set the pause value of an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @param	pause		The pause value to set.
	*/
	virtual void PauseAudioSource(const std::uint32_t index, const bool pause) = 0;

	/**
	* Get the pause value of an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @return				The pause value of the Audio Source component.
	*/
	virtual bool GetAudioSourcePaused(const std::uint32_t index) = 0;


	/**
	* Set the spatialization of an Audio Source component.
	* @param	index				The index of the ChannelGroup of the Audio Source component.
	* @param	spatialization		The spatialization value to set.
	*/
	virtual void SetAudioSourceSpatialization(const std::uint32_t index, const ChannelSpatialization spatialization) = 0;

	/**
	* Get the spatialization value of an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @return				The spatialization value of the Audio Source component.
	*/
	virtual ChannelSpatialization GetAudioSourceSpatialization(const std::uint32_t index) = 0;
	

	/**
	* Set the volume of an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @param	volume		The volume value to set. Between 0 and 1.
	*/
	virtual void SetAudioSourceGroupVolume(const std::uint32_t index, const float volume) = 0;

	/**
	* Get the volume value of an Audio Source component.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @return				The volume value of the Audio Source component.
	*/
	virtual float GetAudioSourceGroupVolume(const std::uint32_t index) = 0;
	

	/**
	* Set the position in world space of an Audio Source component. It must've been loaded with a 3D ChannelGroup.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @param	poition		The position value to set.
	*/
	virtual void SetAudioSourceGroupPos(const std::uint32_t index, const Vector3 position) = 0;

	/**
	* Get the position in world space of an Audio Source component. It must've been loaded with a 3D ChannelGroup.
	* @param	index		The index of the ChannelGroup of the Audio Source component.
	* @return				The position in world space of the Audio Source component.
	*/
	virtual Vector3 GetAudioSourceGroupPos(const std::uint32_t index) = 0;



// -----------------------------------------------------------------------------
//                 Instant Play Sound
// -----------------------------------------------------------------------------

	/**
	* Play a 2D sound without an Audio Source component.
	* @param	sound		The sound to play. It must've been loaded at a 2D sound.
	* @param	volume		The volume value to play the sound with. Between 0 and 1.
	* @param	loop		The number of time the sound will loop. -1 = infinite loop. 0 = play only once (defaut). The sound must've been loaded with the loop settings.
	*/
	virtual void InstantPlaySound2D(const AudioSound& sound, const float volume = 1.0f, const int loop = 0) = 0;

	/**
	* Play a 3D sound without an Audio Source component.
	* @param	sound			The sound to play. It must've been loaded at a 3D sound.
	* @param	playPosition	The position in world space the sound will be located.
	* @param	volume			The volume value to play the sound with. Between 0 and 1.
	* @param	loop			The number of time the sound will loop. -1 = infinite loop. 0 = play only once (defaut). The sound must've been loaded with the loop settings.
	*/
	virtual void InstantPlaySound3D(const AudioSound& sound, const Vector3 playPosition, const float volume = 1.0f, const int loop = 0) = 0;



// -----------------------------------------------------------------------------
//                 Collisions (Geometry)
// -----------------------------------------------------------------------------

	/**
	* Create Geometry for a Collision component.
	* @param	maxPolygons		The maximum number of polygons the geometry of this collision can have.
	* @param	maxVertices		The maximum number of vertices the geometry of this collision can have.
	* @return					The index of the created Geometry.
	*/
	virtual std::uint32_t CreateCollision(const int maxPolygons, const int maxVertices) = 0;

	/**
	* Release a Geometry for a Collision component destruction.
	* @param	index		The index of the Geometry of the Collision component.
	*/
	virtual void ReleaseCollision(const std::uint32_t index) = 0;


	/**
	* Add a polygon to the Geometry of a Collision component.
	* @param	index					The index of the Geometry of the Collision component.
	* @param	audioCollisionType		The type of surface this polygon must have. Will determine the occlusion of the sound by this polygon.
	* @param	doubleSided				Does this polygon occlude sound from both sides or not.
	* @param	vertices				The vertices of the polygon. In the local space of the Geometry.
	*/
	virtual void AddPolygonToCollision(const std::uint32_t index, const AudioCollisionOcclusion& audioCollisionType, const bool doubleSided, const std::vector<Vector3> vertices) = 0;

	/**
	* Set the transform in world space of the Geometry of a Collision component.
	* @param	index		The index of the Geometry of the Collision component.
	* @param	transform	The transform to set. Will apply position and scale, but not rotation. (This may change later and is due to this engine collisions being only AABB for the moment.)
	*/
	virtual void SetCollisionTransform(const std::uint32_t index, const Transform& transform) = 0;



// -----------------------------------------------------------------------------
//                 Audio Management
// -----------------------------------------------------------------------------

	/**
	* Initialize the audio system.
	* Automatically called by the engine when creating the audio system. Prefer to not call this function.
	* @param	maxWorldSize	The maximum world size for geometry. Unused for the moment.
	* @return					True if the initialization was successful.
	*/
	virtual bool Initialize(const float maxWorldSize) = 0;

	/**
	* Exit the audio system to properly release the audio system before destroying it.
	* Automatically called by the engine when deleting the audio system. Prefer to not call this function.
	*/
	virtual void Quit() = 0;

	/**
	* Update the audio system.
	* Automatically called by the engine each frame. Prefer to not call this function.
	*/
	virtual void Update() = 0;

	/**
	* Update the position of the audio listener (generally the active camera).
	* Automatically called by the engine each frame. Prefer to not call this function.
	* @param	listenerPos			The position of the listener.
	* @param	listenerUp			The up vector of the listener.
	* @param	listenerForward		The forward vector of the listener.
	*/
	virtual void UpdateListener(const Vector3 listenerPos, const Vector3 listenerUp, const Vector3 listenerForward) = 0;

	/**
	* Pause all currently playing sounds.
	* Automatically called when pausing the engine. Prefer to not call this function.
	*/
	virtual void PauseAll() = 0;

	/**
	* Resume all currently playing sounds.
	* Automatically called when un-pausing the engine. Prefer to not call this function.
	*/
	virtual void ResumeAll() = 0;
};