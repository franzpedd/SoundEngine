#pragma once

#include <AL/al.h>
#include <vector>

namespace Cosmos
{
	class SoundBank
	{
	public:

		// constructor
		SoundBank();
		
		// destructor
		~SoundBank();

	public:

		// adds a new sound to the sound bank
		ALuint AddSound(const char* filename);

		// removes the sound from the sound bank
		bool RemoveSound(const ALuint& buffer);

	private:

		std::vector<ALuint> mSoundBank = {};
	};
}