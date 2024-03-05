#include "Sound.h"

#include "AudioCore.h"

#include <iostream>

namespace Cosmos::sound
{
	Sound::Sound(std::string path)
	{
		mSoundConfig = ma_sound_config_init();
		mSoundConfig.pFilePath = path.c_str();
		mSoundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;

		if (ma_sound_init_ex(&AudioCore::Get()->GetEngine(), &mSoundConfig, &mSound) != MA_SUCCESS)
		{
			std::cout << "Failed to initialize the sound" << std::endl;
		}

	}

	Sound::~Sound()
	{
		ma_sound_uninit(&mSound);
	}

	void Sound::Play()
	{
		ma_sound_start(&mSound);
	}

	void Sound::Stop()
	{
		ma_sound_stop(&mSound);
	}

	void Sound::SetVolume(float volume)
	{
		ma_sound_set_volume(&mSound, volume);
	}

	void Sound::SetPosition(float x, float y, float z)
	{
		ma_sound_set_position(&mSound, x, y, z);
	}
}
