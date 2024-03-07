#pragma once

#include <AL/alc.h>

namespace Cosmos
{
	class SoundDevice
	{
	public:

		// returns the sound device singleton
		static SoundDevice* Get();

	public:

		// constructor
		SoundDevice();

		// destructor
		~SoundDevice();

	private:

		static SoundDevice* sSoundDevice;
		ALCdevice* mDevice = nullptr;
		ALCcontext* mContext = nullptr;
	};
}