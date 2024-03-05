#pragma once

#include "miniaudio/miniaudio.h"
#include <string>

namespace Cosmos::sound
{
	class Sound
	{
	public:

		// constructor
		Sound(std::string path);

		// destructor
		~Sound();

	public:

		// plays the sound
		void Play();

		// stops the sound
		void Stop();

		// modifies the current sound volume
		void SetVolume(float volume);

		// changes the sound position
		void SetPosition(float x, float y, float z);

	private:

		ma_sound mSound = {};
		std::string mPath = {};
		float mPosition[3] = { 0.0f, 0.0f, 0.0f };
		float mVolume = 1.0f;

		ma_sound_config mSoundConfig = {};
	};
}