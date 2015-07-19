#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "aicomponent.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "SCMLComponent.h"
#include <vector>
#include "ParticleEmitter.h"

class PlayerComponent :	public AiComponent 
{
public:
	enum Weapon
	{
		STAFF = 1, 
		GUN = 2,
		BAZOOKA = 3,
		NONE
	};


	PlayerComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();


	void draw(Camera* camera, sf::RenderTarget* target);
	void debugDraw(Camera* camera, sf::RenderTarget* target);

	void initialize(GameObject* _parent);
	void shutdown();
	void update();

	void onStateChange();

	void setAnimation(AnimationComponent* _animation);

	void sendMessage(Message& msg);

	void setNumBubbles(float fast, float slow);
	sf::Vector2f getNumBubbles();

	void setCurrWeapon(Weapon weapon);
	Weapon getCurrWeapon();


	void die(/*GameObject* murderer*/float impulse, b2Vec2 point);


private:

	void updatePhysics();
	void updateAnimation();

	void getArmBone();
	void getArmPosition();


	bool active;

	float MAX_X_SPEED;
	float ACCELERATION;
	float BREAK;
	float AIR_BREAK;
	float AIR_FRICTION;
	float MAX_JUMP_HEIGHT;
	float MIN_JUMP_HEIGHT;
	float SLOW_GLIDE;
	float MAX_BUBBLE_CHARGE;
	float MIN_BUBBLE_SIZE;
	float MAX_BUBBLE_SIZE;
	float MIN_BUBBLE_DURATION;
	float MAX_BUBBLE_DURATION;

	ParticleEmitter dustEmitter;

	SCML::Entity::Animation::Mainline::Key::Bone_Ref* armBone;
	sf::Vector2f armPosition;
	GameObject* arm;
	SCMLComponent* armScml;

	AnimationComponent* animation;

	Animation idleAnimation;
	Animation walkAnimation;
	Animation jumpAnimation;
	Animation fallAnimation;

	SCMLComponent* scml;

	Weapon currWeapon;

	float jumpCd;
	bool isJumping;
	int walking;
	
	int numFootContacts;
	bool isGrounded;

	float highJumpVelocity;
	float lowJumpVelocity;


	float numFastBubble;
	float numSlowBubble;

	void createBubble(State state,float timer, GameObject* owner = 0);
	void destroyBubble(int i);

	float bubbleTimer;
	bool creatingBubble;
	State bubbleType;

	//int currBubble;
	std::vector<GameObject*> timeBubble;
	
	float deathTimer;
	float winTimer;

	bool star;

};



#endif