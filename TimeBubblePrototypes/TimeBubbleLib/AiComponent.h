#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H
#include "Message.h"
#include <Box2D\Box2D.h>
#include "RendererComponent.h"
#include "GameObject.h"

//class GameObject;
class Prototype;

enum State;

class AiComponent
{
public:

	virtual ~AiComponent() {};

	virtual void write(std::fstream& stream);
	virtual void read(std::fstream& stream, int versionNumber);

	virtual ComponentTypes getType();

	virtual AiComponent* clone() = 0;

	virtual void draw(Camera* camera, sf::RenderTarget* target) {}
	virtual void debugDraw(Camera* camera, sf::RenderTarget* target) {}

	virtual void initialize(GameObject* _parent);
	virtual void shutdown();
	virtual void update();

	virtual void onStateChange() {}

	void setParent(GameObject* _parent);
	GameObject* getParent();

	virtual void sendMessage(Message& msg) {}

protected:
	GameObject* parent;
	Prototype* getGame();
	RendererComponent* getRenderer();

	b2Body* getBody();


};

#endif