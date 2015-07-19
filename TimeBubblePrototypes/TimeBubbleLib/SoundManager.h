#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML\Audio.hpp>
#include <map>

class SoundManager
{
public:

	sf::SoundBuffer& getSound(std::string fileName);

	void load(std::string fileName);
	void unLoad(std::string fileName);

	static SoundManager* getInstance();

private:
	static SoundManager instance;

	SoundManager() {};
	SoundManager(SoundManager& SoundManager);
	SoundManager& operator=(const SoundManager& soundManager);

	std::map<std::string, sf::SoundBuffer> mSounds;
};

#endif