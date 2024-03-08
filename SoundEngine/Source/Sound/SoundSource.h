#pragma once

#include <AL/al.h>

namespace Cosmos
{
	class SoundSource
	{
	public:

		// constructor
		SoundSource(ALuint sound);

		// destructor
		~SoundSource();

	public:

		// plays the sound
		void Start();

		// stops the sound 
		void Stop();

		// pauses the sound
		void Pause();

		// unpauses the sound
		void Unpause();

	private:

		ALenum mState = AL_NONE;
		ALuint mBuffer = 0;
		ALuint mSource = 0;
		float mPitch = 1.0f;
		float mGain = 1.0f;
		float mPosition[3] = { 0.0f, 0.0f, 0.0f };
		float mVelocity[3] = { 0.0f, 0.0f, 0.0f };
		bool mLoop = false;

	};
}