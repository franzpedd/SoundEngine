#include <iostream>

#include "Sound/SoundDevice.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello World" << std::endl;

	Cosmos::SoundDevice* soundDevice = Cosmos::SoundDevice::Get();

	return 0;
}