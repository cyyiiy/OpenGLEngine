#pragma once
#include <ECS/behaviorComponent.h>
#include <Events/observer.h>
#include <Audio/audioUtils.h>
#include <Maths/Vector3.h>

class AudioSound;


/** Audio Source Component
* Component for a persistant Audio Source that can play sounds.
* Default spatialization of the Audio Source is 3D.
*/
class AudioSourceComponent : public BehaviorComponent, public Observer
{
public:
	/**
	* Play a sound on this Audio Source component.
	* @param	sound		The sound to play. It must've been loaded with the same spatialization than this component.
	* @param	loop		The number of time the sound will loop. -1 = infinite loop. 0 = play only once (defaut). The sound must've been loaded with the loop settings.
	*/
	void playSound(const AudioSound& sound, const int loop = 0);

	/**
	* Stop every sound playing on this Audio Source component.
	*/
	void stopSound() const;


	/**
	* Set the pause value.
	* @param	pause		The pause value to set.
	*/
	void setPause(const bool pause);

	/**
	* Get the pause value.
	* @return		The pause value.
	*/
	bool getPaused() const;


	/**
	* Set the spatialization value.
	* @param	spatialization		The spatialization value to set.
	*/
	void setSpatialization(const ChannelSpatialization spatialization);

	/**
	* Get the spatialization value.
	* @return		The spatialization value.
	*/
	ChannelSpatialization getSpatialization() const;


	/**
	* Set the volume.
	* @param	volume		The volume value to set. Between 0 and 1.
	*/
	void setVolume(const float volume);

	/**
	* Get the volume value.
	* @return		The volume value.
	*/
	float getVolume() const;


	/**
	* Set the offset position from owner Entity. It must've been created with the 3D spatialization.
	* @param	poition		The offset value to set.
	*/
	void setOffset(const Vector3 offset);

	/**
	* Get the offset position from owner Entity. It must've been created with the 3D spatialization.
	* @return		The offset value.
	*/
	Vector3 getOffset() const;


	std::uint32_t getChannelIndex() const { return channelIndex; }


	void init() override;
	void exit() override;


private:
	void onEntityMoved();

	std::uint32_t channelIndex{ std::numeric_limits<std::uint32_t>::max() };
	ChannelSpatialization sourceSpatialization{ ChannelSpatialization::Channel3D };

	Vector3 posOffset{ Vector3::zero };

	class Audio* audioManagerRef{ nullptr };
};

