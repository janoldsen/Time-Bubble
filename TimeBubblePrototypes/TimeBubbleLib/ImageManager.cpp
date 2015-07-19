#include "ImageManager.h"
#include <iostream>
#include <fstream>

ImageManager ImageManager::instance;

ImageManager* ImageManager::getInstance()
{
	return &instance;
}


sf::Texture& ImageManager::getTexture(std::string fileName)
{

	//fileName = "res\\" + fileName;


	if (mTextures.find(fileName) != mTextures.end())
	{
		return mTextures.find(fileName)->second;
	}
	else
	{
		std::cout << "loaded " << fileName << std::endl;

		sf::Texture temp;
		temp.setRepeated(true);

		if (temp.loadFromFile(fileName))
		{
			mTextures.insert(std::pair<std::string, sf::Texture>(fileName, temp));

			return mTextures.find(fileName)->second;
		}
		else
		{
			if (mTextures.find("empty") != mTextures.end())
				return mTextures.find("empty")->second;
			else
			{
				sf::Image image;
				image.create(0,0);
				temp.loadFromImage(image);
				mTextures.insert(std::pair<std::string, sf::Texture>("empty", temp));
				return mTextures.find("empty")->second;
			}
		}
	}



}





SCML::Data* ImageManager::getData(std::string fileName)
{

	//fileName = "res\\" + fileName;


	if (mTextures.find(fileName) != mTextures.end())
	{
		return &mDatas.find(fileName)->second;
	}
	else
	{
		std::cout << "loaded " << fileName << std::endl;

		mDatas.insert(std::pair<std::string, SCML::Data>(fileName, SCML::Data()));

		SCML::Data* data = &mDatas.find(fileName)->second;

		data->load(fileName);


		SCML_SFML::FileSystem fs;
		fs.load(data);

		return data;

	}



}


std::string ImageManager::getFileName(const sf::Texture* texture)
{
	std::map<std::string, sf::Texture>::iterator it;

	for (it = mTextures.begin(); it != mTextures.end(); it++)
	{
		if (&it->second == texture)
			return it->first;
	}


	return "";


}





void ImageManager::load(std::string fileName)
{
	std::ifstream in;
	in.open(fileName);

	std::string name;

	while(std::getline(in, name))
	{
		if (std::string(name.end()-4, name.end()) == "scml")
		{
			getData(name);
		}
		else
		{
			getTexture(name);
		}
	}
		
}






void ImageManager::unLoad(std::string fileName)
{
	std::ifstream in;
	in.open(fileName);

	std::string name;

	while(std::getline(in, name))
	{
		if (std::string(name.end()-4, name.end()) == "scml")
		{

			SCML::Data* data = &mDatas.find(name)->second;

			SCML_STRING basedir = std::string(name.begin(), name.begin() + name.find_last_of('/'));

			SCML_BEGIN_MAP_FOREACH_CONST(data->folders, int, SCML::Data::Folder*, folder)
			{
				SCML_BEGIN_MAP_FOREACH_CONST(folder->files, int, SCML::Data::Folder::File*, file)
				{
					if(file->type == "image")
					{
						mTextures.erase(basedir + '/' + file->name);
						std::cout << "unloaded " << basedir + '/' + file->name << std::endl;

					}
				}
				SCML_END_MAP_FOREACH_CONST;
			}
			SCML_END_MAP_FOREACH_CONST;


			mDatas.erase(name);
			
		}
		else
		{
			mTextures.erase(name);
			std::cout << "unloaded " << name << std::endl;
		}
	}
		
}

