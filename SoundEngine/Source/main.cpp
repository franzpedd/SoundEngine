#include <iostream>
#include <Windows.h> // for Sleep

#include "Sound/Listener.h"
#include "Sound/Source.h"

int main(int argc, char* argv[])
{
	Cosmos::sound::Listener::GetInstance();

	Cosmos::sound::Source pinkPanther("../Data/pink_panther.wav");
	pinkPanther.Start();

	Sleep(60000); // 1min

	return 0;
}