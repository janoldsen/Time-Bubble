#ifndef TIME_BUBBLE_COMPONENT
#define TIME_BUBBLE_COMPONENT

#include <Box2D\Box2D.h>
#include "AiComponent.h"
#include <vector>

class TimeBubbleComponent : public AiComponent
{
public:
	TimeBubbleComponent();

	void initialize(GameObject* _parent);

	AiComponent* clone();

	void update();

	void draw(Camera* camera, sf::RenderTarget* target);

	void sendMessage(Message& msg);

	ComponentTypes getType();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	void shutdown();
	void pop();


	void setType(State _type);
	void setOwner(GameObject* _owner);
	void setPlayer(GameObject* _player);

	void setLifeTime(float _lifeTime);
	float getLifeTime();
	void setActive(bool _active);

	void setRadius(float _radius);
	float getRadius() const;


private:
	std::vector<GameObject*> affected;
	std::vector<int> numFix;
	State type;
	GameObject* owner;
	GameObject* player;
	float lifeTime;
	
	float radius;

	bool active;

	bool alive;


};

#endif