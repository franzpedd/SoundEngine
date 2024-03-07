#include "SoundDevice.h"

#include <AL/al.h>
#include <iostream>

namespace Cosmos
{
	SoundDevice* SoundDevice::sSoundDevice = nullptr;;

	SoundDevice::SoundDevice()
	{
		mDevice = alcOpenDevice(nullptr);

		if (!mDevice)
			throw("Failed to get sound device");

		mContext = alcCreateContext(mDevice, nullptr);
		
		if (!mContext)
			throw("Failed to create context");

		if (!alcMakeContextCurrent(mContext))
			throw("Failed to make context current");

		const ALCchar* name = nullptr;

		if (alcIsExtensionPresent(mDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(mDevice, ALC_ALL_DEVICES_SPECIFIER);

		if (!name || alcGetError(mDevice) != AL_NO_ERROR)
			name = alcGetString(mDevice, ALC_DEVICE_SPECIFIER);

		std::cout << "SoundDevice: Opened " << name << std::endl;
	}

	SoundDevice::~SoundDevice()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
	}

	SoundDevice* SoundDevice::Get()
	{
		if (sSoundDevice == nullptr)
		{
			sSoundDevice = new SoundDevice();
		}

		return sSoundDevice;
	}
}