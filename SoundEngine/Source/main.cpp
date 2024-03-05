#include <iostream>

#include "Sound/AudioCore.h"
#include "Sound/Sound.h"

int main(int argc, char* argv[])
{
    std::shared_ptr<Cosmos::sound::AudioCore> audioCore = std::make_shared<Cosmos::sound::AudioCore>();

    Cosmos::sound::Sound windSound("../Data/wind.mp3");

    std::cout << "Hello World" << std::endl;

    return 0;
}
