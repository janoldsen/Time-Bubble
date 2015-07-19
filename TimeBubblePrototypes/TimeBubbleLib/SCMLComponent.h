#ifndef SCML_COMPONENT_H
#define SCML_COMPONENT_H


#include "RendererComponent.h"
#include "GameObject.h"
//#include <SCML\SCMLpp.h>
//#include <SCML\SCML_SFML.h>

#include "SCMLpp.h"
#include "SCML_SFML.h"


using namespace SCML_SFML;

class SCMLComponent : public RendererComponent
{
public:

	SCMLComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int levelVersion);

	ComponentTypes getType();

	RendererComponent* clone();

	void initialize();
	void shutdown();


	void setScml(std::string filePath);
	std::string getFilePath();

	void update(float timeScale);
	void draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color = sf::Color::White);

	void setOrientation(Orientation _orientation);

	void setBaseOrientation(Orientation orientation);
	Orientation getBaseOrientation() const;

	void playAnimation(std::string name);
	void playAnimation(int id);

	std::string getAnimation();


	void setScale(sf::Vector2f _scale);
	sf::Vector2f getScale();

	void sendMessage(Message& msg);

	Entity* getEntity();


private:
	
	FileSystem fs;

	SCML::Data* data;
	Entity* entity;

	Orientation orientation;
	Orientation baseOrientation;
	sf::Vector2f scale;


};



#endif

