#include "SoundBank.h"

#include <AL/alext.h>
#include <iostream>
#include <filesystem>

namespace Cosmos
{
	SoundBank::SoundBank()
	{
	}

	SoundBank::~SoundBank()
	{
        for (auto& sound : mSoundBank)
        {
            alDeleteBuffers(1, &sound.second);
        }
		
		mSoundBank.clear();
	}

	ALuint SoundBank::AddSound(const char* filename)
	{
        std::unordered_map<const char*, ALuint>::const_iterator it = mSoundBank.find(filename);

        if (it != mSoundBank.end())
        {
            std::cout << "Sound " << filename << " already exists on the sound bank" << std::endl;
            return 0;
        }

        SF_INFO info = {};
        SNDFILE* file = sf_open(filename, SFM_READ, &info);
        if (!file)
        {
            std::cout << "Could not open audio " << filename << " Error: " << sf_strerror(file) << std::endl;
            return 0;
        }

        if (info.frames < 1)
        {
            std::cout << "Bad sample count in" << filename << " Count: " << info.frames << std::endl;
            sf_close(file);
            return 0;
        }

        SoundFormat soundFormat = GetSoundFormat(info);
        ALint byteBlockAlign = 0;
        ALint splBlockAlign = 0;


        if (soundFormat == IMA4 || soundFormat == MSADPCM)
        {
            // for ADPCM, look the wave "fmt " chunk
            SF_CHUNK_INFO chunkInfo = { "fmt ", 4, 0, nullptr };
            SF_CHUNK_ITERATOR* chunkIterator = sf_get_chunk_iterator(file, &chunkInfo);

            // if there's an error getting the chunk or block alignment, load as INT16 and have libsndfile do the conversion
            if (!chunkIterator || sf_get_chunk_size(chunkIterator, &chunkInfo) != SF_ERR_NO_ERROR || chunkInfo.datalen < 14)
                soundFormat = INT16;

            else
            {
                ALubyte* fmtbuffer = (ALubyte*)calloc(chunkInfo.datalen, 1);
                chunkInfo.data = fmtbuffer;

                if (sf_get_chunk_data(chunkIterator, &chunkInfo) != SF_ERR_NO_ERROR)
                    soundFormat = INT16;

                else
                {
                    // read blockAlign filed and convert from bytes to samples-per-block (veryfing if it's valid by converting back and comparing with the original value)
                    byteBlockAlign = fmtbuffer[12] | (fmtbuffer[13] << 8);
                    if (soundFormat == IMA4)
                    {
                        splBlockAlign = (byteBlockAlign / info.channels - 4) / 4 * 8 + 1;
                        if (splBlockAlign < 1 || ((splBlockAlign - 1) / 2 + 4) * info.channels != byteBlockAlign)
                            soundFormat = INT16;
                    }
                    else
                    {
                        splBlockAlign = (byteBlockAlign / info.channels - 7) * 2 + 2;
                        if (splBlockAlign < 2 || ((splBlockAlign - 2) / 2 + 7) * info.channels != byteBlockAlign)
                            soundFormat = INT16;
                    }
                }
                free(fmtbuffer);
            }
        }

        if (soundFormat == INT16)
        {
            splBlockAlign = 1;
            byteBlockAlign = info.channels * 2;
        }

        else if (soundFormat == FLOAT)
        {
            splBlockAlign = 1;
            byteBlockAlign = info.channels * 4;
        }

        // get openal format from file and desired sound format
        ALenum format = GetOpenALFormat(file, soundFormat, info.channels);

        if (!format)
        {
            std::cout << "Unsupported channel count:" << info.channels << std::endl;
            sf_close(file);
            return 0;
        }

        if (info.frames / splBlockAlign > (sf_count_t)(INT_MAX / byteBlockAlign))
        {
            std::cout << "Too many samples in " << filename << " Samples: " << info.frames << std::endl;
            sf_close(file);
            return 0;
        }

        // decode the audio file to a buffer
        void* memory = malloc((size_t)(info.frames / splBlockAlign * byteBlockAlign));
        sf_count_t frames = {};

        if (soundFormat == INT16)
            frames = sf_readf_short(file, (short*)memory, info.frames);

        else if (soundFormat == FLOAT)
            frames = sf_readf_float(file, (float*)memory, info.frames);

        else
        {
            sf_count_t count = info.frames / splBlockAlign * byteBlockAlign;
            frames = sf_read_raw(file, memory, count);

            if (frames > 0)
                frames = frames / byteBlockAlign * splBlockAlign;
        }

        if (frames < 1)
        {
            free(memory);
            sf_close(file);

            std::cout << "Failed to read samples in " << filename << " Samples: " << frames << std::endl;
            return 0;
        }

        ALsizei bytes = (ALsizei)(frames / splBlockAlign * byteBlockAlign);
        std::cout << "Loading: " << filename << " Format: " << GetFormatName(format) << " Sample Rate: " << info.samplerate << "hz" << std::endl;

        // generate buffer
        ALuint bufferID = 0;
        alGenBuffers(1, &bufferID);

        if (splBlockAlign > 1)
            alBufferi(bufferID, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splBlockAlign);
        alBufferData(bufferID, format, memory, bytes, info.samplerate);

        // free memory
        free(memory);
        sf_close(file);

        // check final errors
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            std::cout << "An error has ocurred: " << alGetString(error) << std::endl;

            if (bufferID && alIsBuffer(bufferID))
                alDeleteBuffers(1, &bufferID);

            return 0;
        }

        mSoundBank.insert({filename, bufferID});
        return bufferID;
	}

	bool SoundBank::RemoveSound(const char* filename)
	{
        std::unordered_map<const char*, ALuint>::const_iterator it = mSoundBank.find(filename);

        if (it != mSoundBank.end())
        {
            alDeleteBuffers(1, &mSoundBank[filename]);
            mSoundBank.erase(filename);
            return true;
        }

		return false;
	}

    ALuint SoundBank::GetSound(const char* filename)
    {
        std::unordered_map<const char*, ALuint>::const_iterator it = mSoundBank.find(filename);

        if (it != mSoundBank.end())
        {
            return mSoundBank[filename];
        }

        return 0;
    }

    SoundFormat GetSoundFormat(SF_INFO& info)
    {
        SoundFormat format = INT16;
        switch (info.format & SF_FORMAT_SUBMASK)
        {
        case SF_FORMAT_PCM_24:
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
        case SF_FORMAT_VORBIS:
        case SF_FORMAT_OPUS:
        case SF_FORMAT_ALAC_20:
        case SF_FORMAT_ALAC_24:
        case SF_FORMAT_ALAC_32:
        case SF_FORMAT_MPEG_LAYER_I:
        case SF_FORMAT_MPEG_LAYER_II:
        case SF_FORMAT_MPEG_LAYER_III:
        {
            if (alIsExtensionPresent("AL_EXT_FLOAT32")) 
                format = FLOAT;

            break;
        }
            
        case SF_FORMAT_IMA_ADPCM:
        {
            if (info.channels <= 2 && (info.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV && alIsExtensionPresent("AL_EXT_IMA4") && alIsExtensionPresent("AL_SOFT_block_alignment")) 
                format = IMA4;

            break;
        }
            
        case SF_FORMAT_MS_ADPCM:
            if (info.channels <= 2 && (info.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV && alIsExtensionPresent("AL_SOFT_MSADPCM") && alIsExtensionPresent("AL_SOFT_block_alignment"))
                format = MSADPCM;

            break;
        }

        return format;
    }

    ALenum GetOpenALFormat(SNDFILE* file, SoundFormat soundFormat, int channels)
    {
        ALuint format = AL_NONE;

        if (channels == 1)
        {
            switch (soundFormat)
            {
            case Cosmos::INT16: { format = AL_FORMAT_MONO16; break; }
            case Cosmos::FLOAT: { format = AL_FORMAT_MONO_FLOAT32; break; }
            case Cosmos::IMA4: { format = AL_FORMAT_MONO_IMA4; break; }
            case Cosmos::MSADPCM: { format = AL_FORMAT_MONO_MSADPCM_SOFT; break; }
            default: break;
            }
        }

        else if (channels == 2)
        {
            switch (soundFormat)
            {
            case Cosmos::INT16: { format = AL_FORMAT_STEREO16; break; }
            case Cosmos::FLOAT: { format = AL_FORMAT_STEREO_FLOAT32; break; }
            case Cosmos::IMA4: { format = AL_FORMAT_STEREO_IMA4; break; }
            case Cosmos::MSADPCM: { format = AL_FORMAT_STEREO_MSADPCM_SOFT; break; }
            default: break;
            }
        }

        else if (channels == 3)
        {
            if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            {
                if (soundFormat == INT16) 
                    format = AL_FORMAT_BFORMAT2D_16;

                else if (soundFormat == FLOAT) 
                    format = AL_FORMAT_BFORMAT2D_FLOAT32;
            }
        }

        else if (channels == 4)
        {
            if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            {
                if (soundFormat == INT16)
                    format = AL_FORMAT_BFORMAT3D_16;

                else if (soundFormat == FLOAT)
                    format = AL_FORMAT_BFORMAT3D_FLOAT32;
            }
        }

        return format;
    }

    const char* GetFormatName(ALenum format)
    {
        switch (format)
        {
        case AL_FORMAT_MONO8: return "Mono, U8";
        case AL_FORMAT_MONO16: return "Mono, S16";
        case AL_FORMAT_MONO_FLOAT32: return "Mono, Float32";
        case AL_FORMAT_MONO_MULAW: return "Mono, muLaw";
        case AL_FORMAT_MONO_ALAW_EXT: return "Mono, aLaw";
        case AL_FORMAT_MONO_IMA4: return "Mono, IMA4 ADPCM";
        case AL_FORMAT_MONO_MSADPCM_SOFT: return "Mono, MS ADPCM";
        case AL_FORMAT_STEREO8: return "Stereo, U8";
        case AL_FORMAT_STEREO16: return "Stereo, S16";
        case AL_FORMAT_STEREO_FLOAT32: return "Stereo, Float32";
        case AL_FORMAT_STEREO_MULAW: return "Stereo, muLaw";
        case AL_FORMAT_STEREO_ALAW_EXT: return "Stereo, aLaw";
        case AL_FORMAT_STEREO_IMA4: return "Stereo, IMA4 ADPCM";
        case AL_FORMAT_STEREO_MSADPCM_SOFT: return "Stereo, MS ADPCM";
        case AL_FORMAT_QUAD8: return "Quadraphonic, U8";
        case AL_FORMAT_QUAD16: return "Quadraphonic, S16";
        case AL_FORMAT_QUAD32: return "Quadraphonic, Float32";
        case AL_FORMAT_QUAD_MULAW: return "Quadraphonic, muLaw";
        case AL_FORMAT_51CHN8: return "5.1 Surround, U8";
        case AL_FORMAT_51CHN16: return "5.1 Surround, S16";
        case AL_FORMAT_51CHN32: return "5.1 Surround, Float32";
        case AL_FORMAT_51CHN_MULAW: return "5.1 Surround, muLaw";
        case AL_FORMAT_61CHN8: return "6.1 Surround, U8";
        case AL_FORMAT_61CHN16: return "6.1 Surround, S16";
        case AL_FORMAT_61CHN32: return "6.1 Surround, Float32";
        case AL_FORMAT_61CHN_MULAW: return "6.1 Surround, muLaw";
        case AL_FORMAT_71CHN8: return "7.1 Surround, U8";
        case AL_FORMAT_71CHN16: return "7.1 Surround, S16";
        case AL_FORMAT_71CHN32: return "7.1 Surround, Float32";
        case AL_FORMAT_71CHN_MULAW: return "7.1 Surround, muLaw";
        case AL_FORMAT_BFORMAT2D_8: return "B-Format 2D, U8";
        case AL_FORMAT_BFORMAT2D_16: return "B-Format 2D, S16";
        case AL_FORMAT_BFORMAT2D_FLOAT32: return "B-Format 2D, Float32";
        case AL_FORMAT_BFORMAT2D_MULAW: return "B-Format 2D, muLaw";
        case AL_FORMAT_BFORMAT3D_8: return "B-Format 3D, U8";
        case AL_FORMAT_BFORMAT3D_16: return "B-Format 3D, S16";
        case AL_FORMAT_BFORMAT3D_FLOAT32: return "B-Format 3D, Float32";
        case AL_FORMAT_BFORMAT3D_MULAW: return "B-Format 3D, muLaw";
        case AL_FORMAT_UHJ2CHN8_SOFT: return "UHJ 2-channel, U8";
        case AL_FORMAT_UHJ2CHN16_SOFT: return "UHJ 2-channel, S16";
        case AL_FORMAT_UHJ2CHN_FLOAT32_SOFT: return "UHJ 2-channel, Float32";
        case AL_FORMAT_UHJ3CHN8_SOFT: return "UHJ 3-channel, U8";
        case AL_FORMAT_UHJ3CHN16_SOFT: return "UHJ 3-channel, S16";
        case AL_FORMAT_UHJ3CHN_FLOAT32_SOFT: return "UHJ 3-channel, Float32";
        case AL_FORMAT_UHJ4CHN8_SOFT: return "UHJ 4-channel, U8";
        case AL_FORMAT_UHJ4CHN16_SOFT: return "UHJ 4-channel, S16";
        case AL_FORMAT_UHJ4CHN_FLOAT32_SOFT: return "UHJ 4-channel, Float32";
        }
        return "Unknown Format";
    }
}