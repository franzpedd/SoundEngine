#pragma once

#include <AL/al.h>
#include <sndfile.h>
#include <algorithm>
#include <unordered_map>

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

		// adds a new sound to the sound bank and return its id
		ALuint AddSound(const char* filename);

		// removes the sound from the sound bank
		bool RemoveSound(const char* filename);

		// returns the sound id of a given filename
		ALuint GetSound(const char* filename);

	private:

		std::unordered_map<const char*, ALuint> mSoundBank = {};
	};

	typedef enum SoundFormat
	{
		UNDEFINED,
		INT16,
		FLOAT,
		IMA4,
		MSADPCM
	} SoundFormat;

	// returns the sound format given it
	SoundFormat GetSoundFormat(SF_INFO& info);

	// returns the openal sound format
	ALenum GetOpenALFormat(SNDFILE* file, SoundFormat soundFormat, int channels);

	// returns the format name given it's enum
	const char* GetFormatName(ALenum format);
}