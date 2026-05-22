#include "audioSourceComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>
#include <Maths/Matrix4.h>


// Play and Stop
void AudioSourceComponent::playSound(const AudioSound& sound, const int loop)
{
	audioManagerRef->PlaySoundOnAudioSource(channelIndex, sound, loop);
}

void AudioSourceComponent::stopSound() const
{
	audioManagerRef->StopAudioSource(channelIndex);
}


// Pause
void AudioSourceComponent::setPause(const bool pause)
{
	audioManagerRef->PauseAudioSource(channelIndex, pause);
}

bool AudioSourceComponent::getPaused() const
{
	return audioManagerRef->GetAudioSourcePaused(channelIndex);
}

void AudioSourceComponent::setSpatialization(const ChannelSpatialization spatialization)
{
	sourceSpatialization = spatialization;
	audioManagerRef->SetAudioSourceSpatialization(channelIndex, spatialization);
}

ChannelSpatialization AudioSourceComponent::getSpatialization() const
{
	return sourceSpatialization;
}


// Volume
void AudioSourceComponent::setVolume(const float volume)
{
	audioManagerRef->SetAudioSourceGroupVolume(channelIndex, volume);
}

float AudioSourceComponent::getVolume() const
{
	return audioManagerRef->GetAudioSourceGroupVolume(channelIndex);
}


// Offset
void AudioSourceComponent::setOffset(const Vector3 offset)
{
	if (sourceSpatialization != ChannelSpatialization::Channel3D) return;

	posOffset = offset;
	onEntityMoved();
}

Vector3 AudioSourceComponent::getOffset() const
{
	if (sourceSpatialization != ChannelSpatialization::Channel3D) return Vector3::zero;

	return posOffset;
}


void AudioSourceComponent::init()
{
	audioManagerRef = &Locator::getAudio();
	channelIndex = audioManagerRef->CreateAudioSourceGroup(ChannelSpatialization::Channel3D);

	getOwner()->onTransformUpdated.registerObserver(this, Bind_0(&AudioSourceComponent::onEntityMoved));
	setUpdateActivated(false);
}

void AudioSourceComponent::exit()
{
	stopSound();

	getOwner()->onTransformUpdated.unregisterObserver(this);
	audioManagerRef->ReleaseAudioSourceGroup(channelIndex);
}

// Auto-update pos
void AudioSourceComponent::onEntityMoved()
{
	if (sourceSpatialization != ChannelSpatialization::Channel3D) return;

	Matrix4 audio_pos_matrix = Matrix4::createTranslation(posOffset) * getOwner()->getModelMatrix();
	Vector3 audio_pos = audio_pos_matrix.getTranslation();

	audioManagerRef->SetAudioSourceGroupPos(channelIndex, audio_pos);
}

