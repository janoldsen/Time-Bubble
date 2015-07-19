#include "PlayerComponent.h"
#include "GameObject.h"
#include "BodyComponent.h"
#include "Message.h"
#include <iostream>
#include "Prototype.h"
#include "Defines.h"
#include "RendererComponent.h"
#include "TimeBubbleComponent.h"
#include "ImageManager.h"
#include <string>
#include "GameClock.h"
#include <cmath>
#include "myMath.h"
#include "RayCastListener.h"


//#define MAX_X_SPEED 15.0f
//#define ACCELERATION 100.0f
//#define AIR_FRICTION 0.5
//#define MAX_JUMP_HEIGHT 4.0f
//#define MIN_JUMP_HEIGHT 2.0f


PlayerComponent::PlayerComponent()
{
	numFastBubble = 10;
	numSlowBubble = 10;
	animation = 0;
	scml = 0;

	currWeapon = BAZOOKA;
}




void PlayerComponent::initialize(GameObject* _parent)
{
	AiComponent::initialize(_parent);
	isGrounded = false;

	numFootContacts = 0;

	std::ifstream configStream;

	configStream.open("playerConfig.txt");

	if (!configStream.is_open())
	{
		std::cout << "could not find \"playerConfig.txt\"!" << std::endl;

		MAX_X_SPEED		= 15.0f;
		ACCELERATION	= 100.0f;
		BREAK			= 5.0f;
		AIR_BREAK		= 1.5;
		AIR_FRICTION	= 0.5;
		MAX_JUMP_HEIGHT = 4.0f;
		MIN_JUMP_HEIGHT = 2.0f;
		SLOW_GLIDE		= 1.5;
	}
	else
	{
		configStream >> std::string();
		configStream >> MAX_X_SPEED;
		configStream >> std::string();
		configStream >> ACCELERATION;
		configStream >> std::string();
		configStream >> BREAK;
		configStream >> std::string();
		configStream >> AIR_BREAK;
		configStream >> std::string();
		configStream >> AIR_FRICTION;
		configStream >> std::string();
		configStream >> MAX_JUMP_HEIGHT;
		configStream >> std::string();
		configStream >> MIN_JUMP_HEIGHT;
		configStream >> std::string();
		configStream >> SLOW_GLIDE;
		configStream >> std::string();
		configStream >> MAX_BUBBLE_CHARGE;
		configStream >> std::string();
		configStream >> MIN_BUBBLE_SIZE;
		configStream >> std::string();
		configStream >> MAX_BUBBLE_SIZE;
		configStream >> std::string();
		configStream >> MIN_BUBBLE_DURATION;
		configStream >> std::string();
		configStream >> MAX_BUBBLE_DURATION;
	}





	highJumpVelocity = sqrt(2.0f * GRAVITY * MAX_JUMP_HEIGHT);
	lowJumpVelocity = sqrt(2.0f * GRAVITY * MIN_JUMP_HEIGHT);

	isJumping = false;

	armPosition = parent->getCenter();

	if (animation = dynamic_cast<AnimationComponent*>(getRenderer()))
	{
		animation->setParent(parent);


		idleAnimation.duration = 1.0f;
		idleAnimation.numberOfFrames = 4;
		idleAnimation.startPosition = sf::Vector2i(0,0);

		walkAnimation.duration = 0.5f;
		walkAnimation.numberOfFrames = 4;
		walkAnimation.startPosition = sf::Vector2i(0,100);

		jumpAnimation.duration = 0.1f;
		jumpAnimation.numberOfFrames = 4;
		jumpAnimation.startPosition = sf::Vector2i(0,200);

		fallAnimation.duration = 0.5f;
		fallAnimation.numberOfFrames = 3;
		fallAnimation.startPosition = sf::Vector2i(0,300);

		animation->loop(&idleAnimation);

	}


	if (scml = dynamic_cast<SCMLComponent*>(getRenderer()))
	{
		scml->playAnimation("idle");

		getArmBone();

		arm = new GameObject();
		armScml = new SCMLComponent();
		switch (currWeapon)
		{
		case PlayerComponent::STAFF:
			armScml->setScml("res/spriter_player/player_staff.scml");
			break;
		case PlayerComponent::GUN:
			armScml->setScml("res/spriter_player/player_gun.scml");
			break;
		case PlayerComponent::BAZOOKA:
			armScml->setScml("res/spriter_player/player_gun.scml");
			break;
		case PlayerComponent::NONE:
			armScml->setScml("res/spriter_player/player_arm.scml");
		}

		armScml->setScale(scml->getScale());
		arm->setRendererComponent(armScml);
		arm->setAngle(0.0f);


		getGame()->addObject(arm);

	}

	bubbleTimer = 0.0f;
	creatingBubble = false;



	getGame()->setPlayer(parent, this);


	deathTimer = 0.0f;
	winTimer = 0.0f;

	getArmPosition();

	parent->setOrientation(RIGHT);


	dustEmitter.setSprite("res/smoke.png");
	dustEmitter.initialize();

	star = false;

}



void PlayerComponent::write(std::fstream& stream)
{
	stream.write((char*)&currWeapon, sizeof(currWeapon));
	stream.write((char*)&numFastBubble, sizeof(numFastBubble));
	stream.write((char*)&numSlowBubble, sizeof(numSlowBubble));
}

void PlayerComponent::read(std::fstream& stream, int versionNumber)
{
	if (versionNumber >= 7)
	{
		stream.read((char*)&currWeapon, sizeof(currWeapon));
		stream.read((char*)&numFastBubble, sizeof(numFastBubble));
		stream.read((char*)&numSlowBubble, sizeof(numSlowBubble));

	}
}

ComponentTypes PlayerComponent::getType()
{
	return ComponentTypes::PLAYER;
}



AiComponent* PlayerComponent::clone()
{
	return new PlayerComponent(*this);
}

static void rotate_point(float& x, float& y, float angle, float origin_x, float origin_y, bool flipped)
{

	if (flipped)
		angle *= -1;

	float s = sinf(angle*b2_pi/180);
	float c = cosf(angle*b2_pi/180);
	float xnew = (x * c) - (y * s);
	float ynew = (x * s) + (y * c);
	xnew += origin_x;
	ynew += origin_y;

	x = xnew;
	y = ynew;
}


void PlayerComponent::draw(Camera* camera, sf::RenderTarget* target)
{
	if (creatingBubble && active && currWeapon != NONE)
	{

		float timer = bubbleTimer;

		if (timer > MAX_BUBBLE_CHARGE)
			timer = MAX_BUBBLE_CHARGE;

		timer /= MAX_BUBBLE_CHARGE;

		float radius = MIN_BUBBLE_SIZE + timer * (MAX_BUBBLE_SIZE - MIN_BUBBLE_SIZE);

		sf::CircleShape bubblePreview;
		bubblePreview.setRadius(radius * mToPx);
		bubblePreview.setOrigin(radius * mToPx, radius * mToPx);
		
		bubblePreview.setFillColor(sf::Color(100,100,100,150));
		bubblePreview.setOutlineColor(sf::Color(50,50,50,255));
		bubblePreview.setOutlineThickness(3.0f);

		sf::View view = getGame()->window.getView();


		sf::Vector2f dir = getGame()->window.mapPixelToCoords(sf::Mouse::getPosition(getGame()->window), getGame()->getCamera()->getView());
		dir = MATH::vPxtoM(dir) - armPosition/*parent->getCenter()*/;



		bool self = (MATH::magnitudeSquared(dir)) < 9.0f;

		if (getGame()->controllerActive())
		{
			dir = sf::Vector2f(sf::Joystick::getAxisPosition(0,sf::Joystick::U),
				-sf::Joystick::getAxisPosition(0, sf::Joystick::R));

			self = (abs(sf::Joystick::getAxisPosition(0, sf::Joystick::U)) < 25.0f && abs(sf::Joystick::getAxisPosition(0, sf::Joystick::R)) < 25.0f);

		}

		sf::Vector2f nDir = MATH::normalize(dir);

		sf::Vector2f position;

		if (!self  && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			dir = nDir * 5.0f;
			RayCastListener r;
			r.type = GROUND;
			getGame()->world->RayCast(&r, MATH::sfTob2(armPosition/*parent->getCenter()*/),MATH::sfTob2(armPosition/*parent->getCenter()*/ + nDir *(5.0f + radius)));

			if (r.fixture)
			{
				position = sf::Vector2f(MATH::b2ToSf(r.point) + sf::Vector2f(r.normal.x * radius, r.normal.y * radius));
			}
			else
			{
				position = sf::Vector2f(dir.x + armPosition.x, dir.y + armPosition.y);
			}

		}
		else
		{
			position = sf::Vector2f(parent->getCenter());
		}

		bubblePreview.setPosition(MATH::vMtoPx(position));

		target->draw(bubblePreview); 

	}
}

void PlayerComponent::debugDraw(Camera* camera, sf::RenderTarget* target)
{
	if (scml)
	{
		sf::CircleShape arm;
		arm.setRadius(8.0f);
		arm.setOrigin(8.0f, 8.0f);
		arm.setFillColor(sf::Color::Green);
		arm.setPosition(MATH::vMtoPx(armPosition));

		target->draw(arm);


	}

}

void PlayerComponent::shutdown()
{
	if (timeBubble.size() > 0)
	{
		Message msg("PLAYER_DEAD");

		for (int i = 0; i < timeBubble.size(); i++)
		{
			if (timeBubble[i])
				timeBubble[i]->sendMessage(msg);
		}

	}


	//if (getGame())
	//	getGame()->setGameOver(-1);

	dustEmitter.shutdown();

	AiComponent::shutdown();

}


void PlayerComponent::setAnimation(AnimationComponent* _animation)
{
	animation = _animation;
}

void PlayerComponent::update()
{

	active = !(deathTimer != 0.0f) && !(winTimer != 0.0f);

	//arm->setPosition(armPosition);
	//arm->setOrientation(parent->getOrientation());


	if (deathTimer != 0.0f)
	{
		if (deathTimer > 0.0f)
			deathTimer -= gameClock->getElapsedTime();

		if (deathTimer < 0.0f)
		{
			deathTimer = 0.0f;
			getGame()->setGameOver(-1);
			getGame()->destroyObject(parent);
		}

		//return;
	}



	if (winTimer != 0.0f)
	{
		if (getBody()->IsActive())
			getBody()->SetActive(false);

		if (winTimer > 0.0f)
			winTimer -= gameClock->getElapsedTime();

		if (winTimer < 0.0f)
		{
			winTimer = 0.0f;
			if (star)
				getGame()->setGameOver(2);
			else
				getGame()->setGameOver(1);
		}

		

		//return;
	}



	if (active)
	{
		float levelBorder = getGame()->getLevel().getSize().top  - getGame()->getLevel().getSize().height;
		if (parent->getPosition().y <= levelBorder)
			die(0, b2Vec2(0.0f,0.0f));

	}




	if (numFootContacts > 0)
		isGrounded = true;
	else
		isGrounded = false;


	if (jumpCd > 0)
		jumpCd -= gameClock->getElapsedTime();
	else if(isGrounded && isJumping)
		isJumping = false;

	updatePhysics();


	if (active && currWeapon != NONE)
	{
		bubbleTimer += gameClock->getElapsedTime();


		bool fast = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool slow = sf::Mouse::isButtonPressed(sf::Mouse::Right);

		if (getGame()->controllerActive())
		{
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) > 10.0f)
				fast = true; 
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -10.0f)
				slow = true;
		}


		if (/*(sf::Mouse::isButtonPressed(sf::Mouse::Left)
			|| sf::Mouse::isButtonPressed(sf::Mouse::Right))*/
			(fast || slow)
			&& !creatingBubble )
		{
			creatingBubble = true;

			if (fast && numFastBubble > 0)
			{
				bubbleType = FAST;
				bubbleTimer = 0.0f;
			}
			else if (slow && numSlowBubble > 0)
			{
				bubbleType = SLOW;
				bubbleTimer = 0.0f;
			}
			else
			{
				creatingBubble = false;
			}


		}
		if(!slow/*sf::Mouse::isButtonPressed(sf::Mouse::Right)*/ && !fast/*sf::Mouse::isButtonPressed(sf::Mouse::Left)*/ && creatingBubble)
		{
			std::cout << bubbleTimer << std::endl;
			createBubble(bubbleType, bubbleTimer);

			if (scml)
				armScml->playAnimation("shot");

			creatingBubble = false;
		}
	}

	//Animation
	updateAnimation();


}

void PlayerComponent::updatePhysics()
{
	if (!active)
		return;

	b2Body* body = getBody();

	b2Vec2 vel = body->GetLinearVelocity();


	walking = 0;

	float maxSpeed = MAX_X_SPEED * parent->getTimescale();


	if (getGame()->controllerActive())
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::U) > 20)
			parent->setOrientation(RIGHT);
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::U) < -20)
			parent->setOrientation(LEFT);
		else
		{
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20)
				parent->setOrientation(RIGHT);
			else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20)
				parent->setOrientation(LEFT);


		}
	}
	else
	{
		sf::Vector2f v = getGame()->window.mapPixelToCoords(sf::Mouse::getPosition(getGame()->window), getGame()->getCamera()->getView());

		if ((v.x - parent->getPosition().x * mToPx) > 0.0)
			parent->setOrientation(RIGHT);
		else
			parent->setOrientation(LEFT);
	}





	bool right = false;
	bool left = false;
	bool jump = false;



	//std::cout << maxSpeed << std::endl;

	if (getGame()->controllerActive())
	{
		//std::cout << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) << std::endl;

		float js = sf::Joystick::getAxisPosition(0, sf::Joystick::X);


		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20)
		{
			right = true;
			if (js > 85.0f)
				js = 85.0f;
			maxSpeed *= js / 85.0f;
		}
		else
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20)
			{
				left = true;

				if (js < -85.0f)
					js = -85.0f;

				maxSpeed *= js / -85.0f;
			}

			if (sf::Joystick::isButtonPressed(0,0) || sf::Joystick::isButtonPressed(0,
				4) || sf::Joystick::isButtonPressed(0,5))
				jump = true;

	}
	else
	{
		right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		jump = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	}

	//std::cout << maxSpeed << std::endl;


	if (right/*sf::Keyboard::isKeyPressed(sf::Keyboard::D)*/)
	{
		if (vel.x <= maxSpeed)
		{
			float force = ACCELERATION * gameClock->getElapsedTime() * 60.0f;
			force *= parent->getTimescale();
			force *= body->GetMass();

			if (!isGrounded)
				force *= AIR_FRICTION;

			//if (getGame()->controllerActive())
			//{
			//	force *= sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f;

			//}


			body->ApplyForceToCenter(b2Vec2(force,0),true);

		}
		walking = 1;
	}
	else if (left/*sf::Keyboard::isKeyPressed(sf::Keyboard::A)*/)
	{
		if (vel.x >= -maxSpeed)
		{
			float force = -ACCELERATION * gameClock->getElapsedTime() * 60.0f;
			force *= parent->getTimescale();
			force *= body->GetMass();

			if (!isGrounded)
				force *= AIR_FRICTION;

			//if (getGame()->controllerActive())
			//{
			//	force *= sf::Joystick::getAxisPosition(0, sf::Joystick::X) / -100.0f;

			//}

			body->ApplyForceToCenter(b2Vec2(force,0),true);

		}
		walking = -1;
	}
	else
	{
		if (isGrounded)
		{
			float force = BREAK * -vel.x * gameClock->getElapsedTime() * 60.0f * parent->getTimescale();
			body->ApplyForceToCenter(b2Vec2(force,0),true);
		}
		else
		{
			float force = AIR_BREAK * -vel.x * gameClock->getElapsedTime() * 60.0f * parent->getTimescale();
			body->ApplyForceToCenter(b2Vec2(force,0),true);
		}

	}


	if (jump/*sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)*/)
	{
		if (!isJumping)
		{
			if (!isGrounded)
			{
				getBody()->ApplyLinearImpulse(b2Vec2(0, -getBody()->GetLinearVelocity().y * getBody()->GetMass()), getBody()->GetWorldCenter(), true);
			}

			getBody()->
				ApplyLinearImpulse(b2Vec2(0, parent->getTimescale() * highJumpVelocity * getBody()->GetMass()), getBody()->GetWorldCenter(), true);
			isJumping = true;
			jumpCd = 0.5f;
			vel = body->GetLinearVelocity();

		}


	}
	else if (isJumping && vel.y >= lowJumpVelocity * parent->getTimescale())
	{
		getBody()->ApplyLinearImpulse(b2Vec2(0, ((lowJumpVelocity * parent->getTimescale()) - vel.y)  * getBody()->GetMass()), getBody()->GetWorldCenter(), true);
	}

	if (parent->getState() == SLOW && vel.y < 0.0f)
	{
		getBody()->ApplyForceToCenter(b2Vec2(0.0f, GRAVITY * parent->getTimescale() *  SLOW_GLIDE * gameClock->getElapsedTime() * 60.0f),true);
	}


	if (walking && isGrounded)
	{
		if (fmod(gameClock->getTotalElapsedTime(), 0.2f) < 0.01)
		{
			Particle dust;
			dust.position = sf::Vector3f(parent->getPosition().x, parent->getPosition().y, 0.0f);
			dust.lifeTime = 0.5f;
			dust.startTime = 0.5f;
			dust.velocity = sf::Vector3f(-vel.x, -vel.y, 0.0f);
			dustEmitter.addParticle(dust);
		}
	}

	dustEmitter.update();

}

void PlayerComponent::updateAnimation()
{

	int _animation;

	if (scml)
		_animation = scml->getEntity()->animation;


	if (active)
	{

		if (isJumping)
		{
			if (animation)
				animation->play(&jumpAnimation);

			if (getBody()->GetLinearVelocity().y < 0.0f)
			{
				if (scml)
				{
					if (scml->getAnimation() == "jump_up_up")
						scml->playAnimation("jump_up_up_idle");
					//if (scml->getAnimation() == "jump_forward_up")
					//	scml->playAnimation("jump_forward_idle");
				}
			}
			else if (scml)
			{
				if (scml->getAnimation() != "jump_forward_up" &&
					scml->getAnimation() != "jump_up_up" &&
					scml->getAnimation() != "back_flip_up")
				{
					if (walking)
					{
						if (walking * (parent->getOrientation() -2) > 0)
							scml->playAnimation("jump_forward_up" );
						else
							scml->playAnimation("back_flip_up" );

					}
					else
						scml->playAnimation("jump_up_up");
				}
			}

		}
		else if (isGrounded && !isJumping)
		{
			if (walking)
			{
				if (animation)
					animation->play(&walkAnimation);

				if (scml)
				{
					if (walking * (parent->getOrientation() -2) > 0)
						scml->playAnimation("run");
					else
						scml->playAnimation("back_walk");

				}
			}
			else
			{
				if (animation)
					animation->loop(&idleAnimation);

				if (scml)
				{
					if (scml->getAnimation() == "jump_up_up_idle")
						scml->playAnimation("jump_up_down");
					else if (scml->getAnimation() == "jump_forward_up")
						scml->playAnimation("jump_forward_down");
					else if (scml->getAnimation() == "run" || scml->getAnimation() == "back_walk")
						scml->playAnimation("run_momentum raus");
					else if (scml->getAnimation() == "back_flip_up")
						scml->playAnimation("back_flip_down");

					//else
					//	scml->playAnimation("idle");
				}
			}
		}

		if (creatingBubble && scml)
			armScml->playAnimation("charge");


	}

	else if (scml->getAnimation() != "death_back" && scml->getAnimation() != "death_front")
	{
		//std::cout << "v:	" << getBody()->GetLinearVelocity().LengthSquared() << std::endl;
		//std::cout << "va:	" << getBody()->GetAngularVelocity() << std::endl;


		if (getBody()->GetLinearVelocity().LengthSquared() >= 1.0f &&
			getBody()->GetAngularVelocity() <= 2.0f)
		{

			float angle1 = atan2(getBody()->GetLinearVelocity().y, getBody()->GetLinearVelocity().x);

			float angle = angle1 - getBody()->GetAngle();
			angle *= 180.0f/b2_pi /** (parent->getOrientation() - 2)*/;

			if (parent->getOrientation() == LEFT)
				angle -= 180;


			angle = abs(fmod(angle, 360.0f));


			//std::cout << angle << std::endl;

			if (angle >= 10 && angle <= 80)
				scml->playAnimation("death_back_test");
			else if (angle >= 100 && angle <= 350)
				scml->playAnimation("death_front_test");
			else
				scml->playAnimation("death_normal_test");

			//arm->setAngle(parent->getAngle() * b2_pi / 180.0f);
			arm->setAngle(0.0f);
		}
		else
			scml->playAnimation("death_normal_test");



	}


	if (scml)
	{
		if (_animation != scml->getEntity()->animation)
		{
			getArmBone();
		}

		getArmPosition();

		if (active)
		{

			float angle = arm->getAngle();

			if (getGame()->controllerActive())
			{
				if (abs(sf::Joystick::getAxisPosition(0, sf::Joystick::R)) > 20 || abs(sf::Joystick::getAxisPosition(0, sf::Joystick::U)) > 20) 
				{
					angle = atan2(-sf::Joystick::getAxisPosition(0, sf::Joystick::R) * (parent->getOrientation() - 2),
						sf::Joystick::getAxisPosition(0, sf::Joystick::U) * (parent->getOrientation() - 2));
				}
			}
			else
			{
				sf::Vector2f v = getGame()->window.mapPixelToCoords(sf::Mouse::getPosition(getGame()->window), getGame()->getCamera()->getView());
				v -= MATH::vMtoPx(armPosition);
				v *= (parent->getOrientation() - 2.0f);

				angle = atan2(-v.y, v.x);

			}

			arm->setAngle(angle);


		}




		arm->setPosition(armPosition);
		arm->setOrientation(parent->getOrientation());



	}





}

void PlayerComponent::onStateChange()
{
	//if (parent->getState() == SLOW)
	//{
	//	if (getBody())
	//	{
	//		getBody()->SetGravityScale(getBody()->GetGravityScale() * SLOW_GLIDE);
	//	}
	//}
}

void PlayerComponent::sendMessage(Message& msg)
{
	if (msg == "IS_GROUNDED")
	{
		numFootContacts++;
		//if (getBody()->GetLinearVelocity().y <= 0 && isJumping)


	}
	if (msg == "IS_NOT_GROUNDED")
		numFootContacts--;

	if (msg == "BUBBLE_DESTROYED")
	{
		for (int i = 0; i < timeBubble.size(); i++)
		{
			if (timeBubble[i] == msg.getArgument(0))
			{
				timeBubble.erase(timeBubble.begin() + i);
			}
		}
	}

	if (msg == "DIE")
	{
		die(*(float32*)msg.getArgument(0), *(b2Vec2*)msg.getArgument(1));
	}

	if (msg == "BEGIN_CONTACT")
	{
		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		GameObject* other = (GameObject*)msg.getArgument(0);

		if (typeB & GROUND && typeA & FOOT)
		{
			numFootContacts++;
			if (getBody()->GetLinearVelocity().y <= 0 && isJumping)
				isJumping = false;


		}
		//if (typeB & DEADLY)
		//	die(other);

		if (typeB & CRUSHING)
		{
			b2WorldManifold* manifold = (b2WorldManifold*)msg.getArgument(3);

			sf::Vector2f p = MATH::b2ToSf(manifold->points[0]);

			if (other->getBody()->GetLinearVelocity().y < 0 && isGrounded && p.y > parent->getCenter().y)
				die(20.0f, b2Vec2(0.0f,0.0f));
		}

	}


	if (msg == "END_CONTACT")
	{
		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		if (typeB & GROUND && typeA & FOOT)
			numFootContacts--;

	}


	if (msg == "ZOOM_TO")
	{
		getGame()->camera.zoomTo(*(float*)msg.getArgument(0), *(float*)msg.getArgument(1));
	}

	if (msg == "LEVEL_END")
	{
		winTimer = 1.5f;

	}

	if (msg == "STAR")
		star = true;

}



void PlayerComponent::die(/*GameObject* murderer*/float impulse, b2Vec2 point)
{

	if (deathTimer != 0.0f)
		return;

	std::cout << impulse << std::endl;



	if (impulse <= 2.0f)
	{
		if (point.x * (parent->getOrientation() -2) < parent->getPosition().x * (parent->getOrientation() -2))
		{
			if (scml)
				scml->playAnimation("death_front");
		}
		else
		{
			if (scml)
				scml->playAnimation("death_back");
		}

	}
	else
	{
		getBody()->SetFixedRotation(false);
		//scml->playAnimation("death_normal_test");
	}


	//if (deathTimer == 0.0f)
	deathTimer = 10.5f;


}





void PlayerComponent::createBubble(State state,float timer, GameObject* owner)
{

	//creatingBubble = true;

	if (timeBubble.size() > currWeapon - 1)
		destroyBubble(0);


	if (timer > MAX_BUBBLE_CHARGE)
		timer = MAX_BUBBLE_CHARGE;

	timer /= MAX_BUBBLE_CHARGE;

	float radius = MIN_BUBBLE_SIZE + timer * (MAX_BUBBLE_SIZE - MIN_BUBBLE_SIZE);

	std::cout << radius << "	" << timer << std::endl;

	if (state == FAST)
		numFastBubble--;
	//numFastBubble -= timer * 1.5f + 0.25f;
	if (state == SLOW)
		numSlowBubble--;
	//numSlowBubble -= timer * 1.5f + 0.25f;


	timeBubble.push_back(new GameObject("TimeBubble"));
	{
		TimeBubbleComponent* ai = new TimeBubbleComponent();
		ai->setType(state);
		ai->setPlayer(parent);
		ai->setOwner(owner);
		ai->setRadius(radius);
		if (state == SLOW)
			ai->setLifeTime((MIN_BUBBLE_DURATION + timer * (MAX_BUBBLE_DURATION - MIN_BUBBLE_DURATION)) * 1.5f);
		else
			ai->setLifeTime(MIN_BUBBLE_DURATION + timer * (MAX_BUBBLE_DURATION - MIN_BUBBLE_DURATION));


		ai->setActive(true);


		SpriteComponent* renderer = new SpriteComponent();
		if (state == SLOW)
			renderer->setSprite(imageManager->getTexture("res\\bubbleSlow.png"));
		if (state == FAST)
			renderer->setSprite(imageManager->getTexture("res\\bubbleFast.png"));

		renderer->setScale(sf::Vector2f(radius/2.0f, radius/2.0f));


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;

		if (currWeapon == STAFF || currWeapon == BAZOOKA)
			bodyDef.gravityScale = 0.0f;

		sf::View view = getGame()->window.getView();


		sf::Vector2f dir = getGame()->window.mapPixelToCoords(sf::Mouse::getPosition(getGame()->window), getGame()->getCamera()->getView());
		dir = MATH::vPxtoM(dir) - armPosition/*parent->getCenter()*/;



		bool self = (MATH::magnitudeSquared(dir)) < 9.0f;

		if (getGame()->controllerActive())
		{
			dir = sf::Vector2f(sf::Joystick::getAxisPosition(0,sf::Joystick::U),
				-sf::Joystick::getAxisPosition(0, sf::Joystick::R));

			std::cout << dir.x << "	" << dir.y << std::endl;

			self = (abs(sf::Joystick::getAxisPosition(0, sf::Joystick::U)) < 25.0f && abs(sf::Joystick::getAxisPosition(0, sf::Joystick::R)) < 25.0f);

		}

		sf::Vector2f nDir = MATH::normalize(dir);

		if (!self/*MATH::magnitudeSquared(dir) > 25.0f*/ && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{

			dir = nDir * 5.0f;




			RayCastListener r;
			r.type = GROUND;
			getGame()->world->RayCast(&r, MATH::sfTob2(armPosition/*parent->getCenter()*/),MATH::sfTob2(armPosition/*parent->getCenter()*/ + nDir *(5.0f + radius)));

			if (r.fixture)
			{
				bodyDef.position = r.point + b2Vec2(r.normal.x * radius, r.normal.y * radius);
			}

			else
			{
				bodyDef.position.x = 	dir.x + /*parent->getCenter()*/armPosition.x;
				bodyDef.position.y = 	dir.y + /*parent->getCenter()*/armPosition.y;


			}

			if (currWeapon == GUN || currWeapon == BAZOOKA)
				bodyDef.linearVelocity = MATH::sfTob2(nDir * (20.0f + 10.0f/((radius * radius)))); 

		}
		else
			bodyDef.position = MATH::sfTob2(parent->getCenter());

		timeBubble.back()->setRendererComponent(renderer);
		timeBubble.back()->setBody(&bodyDef, getGame()->world);
		timeBubble.back()->setAiComponent(ai);

		b2CircleShape shape;
		shape.m_radius = radius;

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;

		//if (state == SLOW)
		//	fixtureDef.userData = new int(SLOWBUBBLE);
		//fixtureDef.userData = new int(BUBBLE);

		fixtureDef.userData = new int (BUBBLE);

		timeBubble.back()->addFixture(&fixtureDef);

	}
	getGame()->addObject(timeBubble.back(), false);


	timeBubble.back()->initialize(getGame());

}

void PlayerComponent::destroyBubble(int i)
{
	Message msg("POP");
	timeBubble[i]->sendMessage(msg);

	getGame()->destroyObject(timeBubble[i]);

	timeBubble.erase(timeBubble.begin() + i);

}



void PlayerComponent::setNumBubbles(float fast, float slow)
{
	numFastBubble = fast;
	numSlowBubble = slow;
}

sf::Vector2f PlayerComponent::getNumBubbles()
{
	return sf::Vector2f(numFastBubble, numSlowBubble);
}


void PlayerComponent::setCurrWeapon(Weapon weapon)
{
	currWeapon = weapon;
}

PlayerComponent::Weapon PlayerComponent::getCurrWeapon()
{
	return currWeapon;
}


void PlayerComponent::getArmBone()
{
	if (!scml)
		return;


	Entity* e = scml->getEntity();
	SCML::Entity::Animation* a = e->getAnimation(e->animation);

	for (int i = 0; i < e->getNumBones(); i++)
	{

		armBone = a->mainline.keys[e->getNextKeyID(e->animation, e->key)]->bones[i].bone_ref;

		std::string name = a->timelines[armBone->timeline]->name;

		if (name == "bone_arm_r_1")
			break;
		armBone = 0;
	}

}

void PlayerComponent::getArmPosition()
{
	if (scml && armBone)
	{
		using namespace SCML;


		SCML_SFML::Entity* e = scml->getEntity();

		float x = mToPx * parent->getPosition().x;
		float y = -mToPx * parent->getPosition().y;
		float angle =- parent->getAngle() * 180.0f/b2_pi * (parent->getOrientation() - 2);

		e->convert_to_SCML_coords(x, y, angle);

		Transform base_transform(x, y, angle, scml->getScale().x * (parent->getOrientation()-2), scml->getScale().y);
		if(e->bone_transform_state.should_rebuild(e->entity, e->animation, e->key, e->time, base_transform))
		{
			e->bone_transform_state.rebuild(e->entity, e->animation, e->key, e->time, e, base_transform);
		}




		SCML::Entity::Animation* animation_ptr = e->getAnimation(e->animation);  // Need this only if looping...
		SCML::Entity::Animation::Timeline::Key* t_key1 = e->getTimelineKey(e->animation, armBone->timeline, armBone->key);
		SCML::Entity::Animation::Timeline::Key* t_key2 = e->getTimelineKey(e->animation, armBone->timeline, armBone->key+1);
		if(t_key2 == NULL)
			t_key2 = t_key1;




		SCML::Entity::Animation::Timeline::Key::Bone* bone1 = &t_key1->bone;
		SCML::Entity::Animation::Timeline::Key::Bone* bone2 = &t_key2->bone;
		if(bone2 == NULL)
			bone2 = bone1;
		if(bone1 != NULL)
		{
			// Get interpolation (tweening) factor
			float t = 0.0f;
			if(t_key2->time > t_key1->time)
				t = (e->time - t_key1->time)/float(t_key2->time - t_key1->time);
			else if(t_key2->time < t_key1->time)
				t = (e->time - t_key1->time)/float(animation_ptr->length - t_key1->time);

			// Get parent bone transform
			Transform parent_transform;
			if(armBone->parent < 0)
				parent_transform = e->bone_transform_state.base_transform;
			else
				parent_transform = e->bone_transform_state.transforms[armBone->parent];

			// Set boneect transform
			Transform bone_transform(bone1->x, bone1->y, bone1->angle, bone1->scale_x, bone1->scale_y);

			// Tween with next key's boneect
			bone_transform.lerp(Transform(bone2->x, bone2->y, bone2->angle, bone2->scale_x, bone2->scale_y), t, t_key1->spin);

			// Transform the sprite by the parent transform.
			bone_transform.apply_parent_transform(parent_transform);


			armPosition.x = bone_transform.x * 1.0f/mToPx;
			armPosition.y = bone_transform.y * 1.0f/mToPx;

		}

	}
	else
	{
		armPosition = parent->getCenter();
	}
}



