#include "AudioCore.h"

#include <iostream>

namespace Cosmos::sound
{
	AudioCore* AudioCore::sAudioCore = nullptr;

	AudioCore::AudioCore()
	{
		if (sAudioCore != nullptr)
		{
			std::cout << "AudioCore has already been initialized, use AudioCore::Get()" << std::endl;
			return;
		}

		sAudioCore = this;

		Initialize();
	}

	AudioCore::~AudioCore()
	{
		if (mContext != nullptr)
		{
			iplContextRelease(&mContext);
		}
		
		ma_engine_uninit(&mEngine);

		sAudioCore = nullptr;
	}

	AudioCore* AudioCore::Get()
	{
		return sAudioCore;
	}

	void AudioCore::Initialize()
	{
		// mini audio initialization
		mEngineConfig = ma_engine_config_init();
		mEngineConfig.channels = CHANNELS;
		mEngineConfig.sampleRate = SAMPLE_RATE;
		mEngineConfig.periodSizeInFrames = PERIOD_SIZE_IN_FRAMES; // this is currently required because of steam audio

		if (ma_engine_init(&mEngineConfig, &mEngine) != MA_SUCCESS)
		{
			std::cout << "Sound: Failed to initialize the sound backend engine" << std::endl;
			return;
		}

		// steam audio initialization
		mAudioSettings.samplingRate = ma_engine_get_sample_rate(&mEngine);
		mAudioSettings.frameSize = mEngineConfig.periodSizeInFrames;
		mContextSettings.version = STEAMAUDIO_VERSION;
		mHRTFSettings.type = IPL_HRTFTYPE_DEFAULT;

		if (iplContextCreate(&mContextSettings, &mContext) != IPL_STATUS_SUCCESS)
		{
			std::cout << "Sound: Failed to create context" << std::endl;
			return;
		}

		if (iplHRTFCreate(mContext, &mAudioSettings, &mHRTFSettings, &mHRTF) != IPL_STATUS_SUCCESS)
		{
			std::cout << "Sound: Failed to create HRTF" << std::endl;
			return;
		}
	}
}