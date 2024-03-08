#include "SoundSource.h"

namespace Cosmos
{
	SoundSource::SoundSource(ALuint sound)
		: mBuffer(sound)
	{
		alGenSources(1, &mSource);
		alSourcef(mSource, AL_PITCH, mPitch);
		alSourcef(mSource, AL_GAIN, mGain);
		alSource3f(mSource, AL_POSITION, mPosition[0], mPosition[1], mPosition[2]);
		alSource3f(mSource, AL_VELOCITY, mVelocity[0], mVelocity[1], mVelocity[2]);
		alSourcei(mSource, AL_LOOPING, mLoop);
		alSourcei(mSource, AL_BUFFER, mBuffer);
	}

	SoundSource::~SoundSource()
	{
		alDeleteSources(1, &mSource);
	}

	void SoundSource::Stop()
	{
		alSourceStop(mSource);
		alGetSourcei(mSource, AL_SOURCE_STATE, &mState);
	}

	void SoundSource::Pause()
	{
		alSourcePause(mSource);
		alGetSourcei(mSource, AL_SOURCE_STATE, &mState);
	}

	void SoundSource::Unpause()
	{
		alSourcePlay(mSource);
		alGetSourcei(mSource, AL_SOURCE_STATE, &mState);
	}

	void SoundSource::Start()
	{
		alSourcePlay(mSource);
		alGetSourcei(mSource, AL_SOURCE_STATE, &mState);
	}
}