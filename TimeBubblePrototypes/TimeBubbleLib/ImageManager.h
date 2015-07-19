#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "SCML_SFML.h"

class ImageManager
{
public:
	
	sf::Texture& getTexture(std::string fileName);
	SCML::Data* getData(std::string fileName);

	std::string getFileName(const sf::Texture* texture);

	
	void load(std::string fileName);
	void unLoad(std::string fileName);

	static ImageManager* getInstance();


private:

	static ImageManager instance;

	ImageManager() {}
	ImageManager(ImageManager& imageManager);
	ImageManager& operator=(const ImageManager& imageManager);

	std::map<std::string, sf::Texture> mTextures; 
	std::map<std::string, SCML::Data> mDatas;
	
};

#define imageManager ImageManager::getInstance()



#endif