#include "SoundBank.h"

#include 
namespace Cosmos
{
	SoundBank::SoundBank()
	{
	}

	SoundBank::~SoundBank()
	{
		alDeleteBuffers(mSoundBank.size(), mSoundBank.data());
		mSoundBank.clear();
	}

	ALuint SoundBank::AddSound(const char* filename)
	{
		ALenum error, format;
		ALuint buffer;
		SNDFILE* sndFile;
		return ALuint();
	}

	bool SoundBank::RemoveSound(const ALuint& buffer)
	{
		return false;
	}
}