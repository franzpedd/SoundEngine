#pragma once

#include <cstdint>
#include "miniaudio/miniaudio.h"
#include <phonon.h>

// stereo audio
#define CHANNELS 2 

// sampling
#define SAMPLE_RATE 48000

// steam audio period size in frames
#define PERIOD_SIZE_IN_FRAMES 256

namespace Cosmos::sound
{
	class AudioCore
	{
	public:

		// constructor
		AudioCore();

		// destructor
		~AudioCore();

		// returns a pointer to the audio core singleton
		static AudioCore* Get();

	public: // mini audio objects

		// returns mini-audio engine
		inline ma_engine& GetEngine() { return mEngine; }

		// returns mini-audio engine configuration
		inline ma_engine_config& GetEngineConfig() { return mEngineConfig; }

	public: // steam audio objects

		// returns steam audio context
		inline IPLContext GetContext() const { return mContext; }

		// returns steam audio context
		inline IPLContextSettings& GetContextSettings() { return mContextSettings; }

		// returns the main head-related transfer function
		inline IPLHRTF GetHRTF() const { return mHRTF; }

		// returns the main head-related transfer function settings
		inline IPLHRTFSettings& GetHRTFSettings() { return mHRTFSettings; }

		// returns the global audio settings (this requires study)
		inline IPLAudioSettings& GetAudioSettings() { return mAudioSettings; }

	private:

		// initializes the global state of the sound engine
		void Initialize();

	private:

		static AudioCore* sAudioCore;

		ma_engine mEngine = {};
		ma_engine_config mEngineConfig = {};

		IPLContext mContext = nullptr;
		IPLContextSettings mContextSettings = {};

		IPLAudioSettings mAudioSettings = {};
		
		IPLHRTF mHRTF = nullptr;
		IPLHRTFSettings mHRTFSettings = {};
	};
}