#include "SwingComponent.h"

#include "GameClock.h"
#include "myMath.h"
#include "Prototype.h"
#include <iostream>


SwingComponent::SwingComponent()
{
	startAngle = 0;

}


void SwingComponent::initialize(GameObject* _parent)
{

	AiComponent::initialize(_parent);

	timer = acos(startAngle/maxAngle) / velocity;
	
	if (getBody())
		getBody()->SetTransform(getBody()->GetPosition(),startAngle);

	scml = dynamic_cast<SCMLComponent*>(getRenderer());

	player = false;
}


AiComponent* SwingComponent::clone()
{
	return new SwingComponent(*this);
}

void SwingComponent::write(std::fstream& stream)
{
	stream.write((char*)&maxAngle, sizeof(maxAngle));
	stream.write((char*)&velocity, sizeof(velocity));
	
	stream.write((char*)&startAngle, sizeof(startAngle));
}

void SwingComponent::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&maxAngle, sizeof(maxAngle));
	stream.read((char*)&velocity, sizeof(velocity));

	if (versionNumber >= 15)
		stream.read((char*)&startAngle, sizeof(startAngle));
}

	
ComponentTypes SwingComponent::getType()
{
	return ComponentTypes::SWING;
}


void SwingComponent::update()
{

	timer += (gameClock->getElapsedTime() * parent->getTimescale());
	
	//float v = -5.0 * cos(timer  * 3) * parent->getTimescale();

	float d = 1.0f;

	if (gameClock->getElapsedTime() > 1.0f/60.0f)
		d = floor(gameClock->getElapsedTime() / (1.0f/60.0f));


	float angle = maxAngle * cos(timer * velocity);

	if (getBody())
		getBody()->SetAngularVelocity(((angle - parent->getAngle()) * 60.0f) / d);


	if (scml)
	{
		if (MATH::magnitudeSquared(getGame()->getPlayer()->getCenter() - parent->getPosition()) <= 500.0f)
		{
			if (!player)
				scml->playAnimation("still");
			player = true;
		}
		else if (player)
		{
			scml->playAnimation("not bad_escape");
			player = false;
		}
	}

}


void SwingComponent::onStateChange()
{
	if (scml)
	{
		if (parent->getState() == FAST)
			scml->playAnimation("rage_fast");
		else if (parent->getState() == SLOW)
			scml->playAnimation("darn u_slow");
		else
			scml->playAnimation("still");
	}

}


void SwingComponent::setMaxAngle(float angle)
{
	maxAngle = angle * b2_pi/180;
}

float SwingComponent::getMaxAngle()
{
	return maxAngle * 180/b2_pi;
}

void SwingComponent::setVelocity(float _velocity)
{
	velocity = _velocity;
}

float SwingComponent::getVelocity()
{
	return velocity;
}


void SwingComponent::sendMessage(Message& msg)
{
	if (msg == "DEACTIVATE" || msg == "DEACTIVATE_AI")
	{
		if (getBody())
			getBody()->SetAngularVelocity(0.0f);

	}

	if (msg == "BEGIN_CONTACT")
	{
		int typeB = (int)msg.getArgument(2);

		if (typeB & PLAYER && scml)
			rand()%2? scml->playAnimation("pft_kill") : scml->playAnimation("pft_ko_kill_2");


	}

}


void SwingComponent::setStartAngle(float angle)
{
	startAngle = angle * b2_pi/180;

	if (startAngle > maxAngle)
		startAngle = maxAngle;

}

float SwingComponent::getStartAngle()
{
	return startAngle * 180/b2_pi;
}