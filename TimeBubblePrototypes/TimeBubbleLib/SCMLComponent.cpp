#include "SCMLComponent.h"
#include "GameClock.h"
#include "Defines.h"
#include "ImageManager.h"



SCMLComponent::SCMLComponent()
{
	data = 0;
	entity = 0;
	orientation = RIGHT;
	baseOrientation = RIGHT;

	scale = sf::Vector2f(1.0f,1.0f);
}



void SCMLComponent::write(std::fstream& stream)
{
	int size = data->name.size();
	stream.write((char*)&size, sizeof(size));
	stream.write(data->name.c_str(), size * sizeof(char));

	stream.write((char*)&scale, sizeof(scale));

	stream.write((char*)&baseOrientation, sizeof(baseOrientation));

}


void SCMLComponent::read(std::fstream& stream, int levelVersion)
{
	int size;

	stream.read((char*)&size, sizeof(size));

	std::string filepath(size, '\0');
	stream.read(&filepath[0], size * sizeof(char));

	setScml(filepath);

	stream.read((char*)&scale, sizeof(scale));

	if (levelVersion >= 12)
		stream.read((char*)&baseOrientation, sizeof(baseOrientation));

}

ComponentTypes SCMLComponent::getType()
{
	return ComponentTypes::SCML;
}

RendererComponent* SCMLComponent::clone()
{
	SCMLComponent* clone = new SCMLComponent();

	//clone->data = new SCML::Data(*data);

	clone->entity = new Entity(data, 0);
	clone->entity->setFileSystem(&fs);


	clone->scale = scale;
	clone->baseOrientation = baseOrientation;


	return clone;

}


void SCMLComponent::initialize()
{

}


void SCMLComponent::shutdown()
{
	if (entity)
		delete entity;

	//if (data)
		//delete data;

}

void SCMLComponent::setScml(std::string filePath)
{
	//if (data)
		//delete data;

	if (entity)
		delete entity;

	data = imageManager->getData(filePath);



	fs.load(data);

	entity = new Entity(data, 0);
	entity->setFileSystem(&fs);



}


void SCMLComponent::update(float timeScale)
{
	if (entity)
		entity->update(gameClock->getElapsedTime() * 1000.0f * timeScale);
}

void SCMLComponent::draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color)
{
	if (entity)
	{
		float o = (orientation - 2) * (baseOrientation - 2);

		entity->setScreen(target);
		entity->draw(position.x * mToPx , position.y * -mToPx, angle * -180.0f / b2_pi * o, scale.x * o, scale.y);
	}
}


void SCMLComponent::setOrientation(Orientation _orientation)
{
	orientation = _orientation;
}

void SCMLComponent::setBaseOrientation(Orientation _orientation)
{
	baseOrientation = _orientation;
}

Orientation SCMLComponent::getBaseOrientation() const
{
	return baseOrientation;
}


void SCMLComponent::playAnimation(std::string name)
{
	entity->animations;
	std::map<int, SCML::Entity::Animation*>::iterator it;

	for(it = entity->animations.begin(); it != entity->animations.end(); it++)
	{
		if (it->second->name == name)
			playAnimation(it->first);
	}
	

}


void SCMLComponent::playAnimation(int id)
{
	if (entity->animation != id)
		entity->startAnimation(id);

}

std::string SCMLComponent::getAnimation()
{
	return entity->animations.find(entity->animation)->second->name;
}


void SCMLComponent::setScale(sf::Vector2f _scale)
{
	scale = _scale;
}

sf::Vector2f SCMLComponent::getScale()
{
	return scale;	
}



std::string SCMLComponent::getFilePath()
{
	if (data)
		return data->name;
	else
		return "";
}


void SCMLComponent::sendMessage(Message& msg)
{
	if (msg == "PLAY_ANIMATION")
	{
		playAnimation(*(float*)msg.getArgument(0));
	}

}


Entity* SCMLComponent::getEntity()
{
	return entity;
}