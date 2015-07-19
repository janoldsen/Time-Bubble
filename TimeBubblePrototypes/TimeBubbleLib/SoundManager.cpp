#include "SoundManager.h"
#include <iostream>
#include <fstream>


SoundManager SoundManager::instance;



SoundManager* SoundManager::getInstance()
{
	return &instance;
}


sf::SoundBuffer& SoundManager::getSound(std::string fileName)
{
	if (mSounds.find(fileName) != mSounds.end())
	{
		return mSounds.find(fileName)->second;
	}
	else
	{
		std::cout<< "loaded " << fileName << std::endl;

		sf::SoundBuffer temp;

		if (temp.loadFromFile(fileName))
		{
			mSounds.insert(std::pair<std::string, sf::SoundBuffer>(fileName, temp));
			return mSounds.find(fileName)->second;
		}
		else
		{
			std::cout << "loading "  << fileName << " failed!" << std::endl;			
		}

	}

}

void SoundManager::load(std::string fileName)
{
	std::ifstream in;
	in.open(fileName);

	std::string name;

	while(std::getline(in, name))
	{
		getSound(name);
	}

}

void SoundManager::unLoad(std::string fileName)
{
	std::ifstream in;

	std::string name;

	while(std::getline(in, name))
	{
		mSounds.erase(name);
	}

}