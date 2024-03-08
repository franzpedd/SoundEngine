#include <iostream>
#include <Windows.h> // for Sleep

#include "Sound/SoundDevice.h"
#include "Sound/SoundBank.h"
#include "Sound/SoundSource.h"

int main(int argc, char* argv[])
{
	Cosmos::SoundDevice* soundDevice = Cosmos::SoundDevice::Get();

	Cosmos::SoundBank soundBank;
	ALuint pinkpanther = soundBank.AddSound("../Data/pink_panther.wav");
	ALuint wind = soundBank.AddSound("../Data/rain.wav");

	Cosmos::SoundSource soundSource(pinkpanther);
	soundSource.Start();

	Sleep(100000); // 100 seconds

	return 0;
}