#include "PlattformComponent.h"
#include "GameObject.h"
#include "GameClock.h"
#include "myMath.h"
#include <math.h>
#include "Defines.h"
#include <iostream>


PlattformComponent::PlattformComponent()
{
	startValue = 0.0f;
	velocity = 1.0f;
	dir = 1;
}


void PlattformComponent::write(std::fstream& stream)
{
	//stream.write((char*)&endPosition, sizeof(endPosition));

	//stream.write((char*)&velocity, sizeof(velocity));

	//stream.write((char*)&startValue, sizeof(startValue));


	stream.write((char*)&velocity, sizeof(velocity));
	stream.write((char*)&startValue, sizeof(startValue));

	stream.write((char*)&startPosition, sizeof(startPosition));
	
	int size = wayPoints.size();
	stream.write((char*)&size, sizeof(size));
	for (int i = 0; i < size; i++)
		stream.write((char*)&wayPoints[i], sizeof(wayPoints[i]));



}


void PlattformComponent::read(std::fstream& stream, int versionNumber)
{
//	stream.read((char*)&endPosition, sizeof(endPosition));
//
//	stream.read((char*)&velocity, sizeof(velocity));
//
//	if (versionNumber >= 15)
//		stream.read((char*)&startValue, sizeof(startValue));

	if (versionNumber < 16)
	{
		wayPoints.push_back(sf::Vector2f(0.0f,0.0f));
		stream.read((char*)&wayPoints.back(), sizeof(wayPoints.back()));

		stream.read((char*)&velocity, sizeof(velocity));

		if (versionNumber >= 15)
			stream.read((char*)&startValue, sizeof(startValue));

	}
	else
	{

		stream.read((char*)&velocity, sizeof(velocity));
		stream.read((char*)&startValue, sizeof(startValue));

		stream.read((char*)&startPosition, sizeof(startPosition));

		int size;
		stream.read((char*)&size, sizeof(size));
		for (int i = 0; i < size; i++)
		{
			wayPoints.push_back(sf::Vector2f());
			stream.read((char*)&wayPoints.back(), sizeof(wayPoints.back()));

		}
	}


}

ComponentTypes PlattformComponent::getType()
{
	return ComponentTypes::PLATTFORM;
}


AiComponent* PlattformComponent::clone()
{
	return new PlattformComponent(*this);
}


void PlattformComponent::initialize(GameObject* _parent)
{
	AiComponent::initialize(_parent);
	
	//wayPoints.insert(wayPoints.begin(), parent->getPosition());
	startPosition = parent->getPosition();
	
	parent->setPosition(startPosition + startValue * (wayPoints[0]- startPosition));



	setPoints(-1, 0);
	


	curr = -1;
	dir = 1;

	//startPosition = parent->getPosition();

	//xScale = 1;
	//yScale = 1;

	//if (startPosition.x > endPosition.x)
	//{
	//	xScale = -1;
	//}
	//if (startPosition.y > endPosition.y)
	//{
	//	yScale = -1;
	//}


	//dirN = MATH::sfTob2(endPosition - parent->getPosition());
	//dirN.Normalize();

	//updateVelocity();

}


void PlattformComponent::onStateChange()
{
	updateVelocity();
}


void PlattformComponent::update()
{
	sf::Vector2f p = parent->getPosition();

	if (getBody())
	{
		if (getBody()->GetLinearVelocity().LengthSquared() == 0.0f)
			updateVelocity();

		if (  (( p.x - pos2.x) * xScale >= 0 && (p.y - pos2.y) * yScale >= 0 ) )
		{
			curr += dir;


			if ((curr + dir)  < -1 || (curr + dir)  >= (int)wayPoints.size())
			{
				dir *= -1;
			}

			setPoints(curr, curr + dir);

			

			//dir = -1;

			//updatevelocity();



		}

		//if ( ( (p.x - startPosition.x) * xScale <= 0 && (p.y - startPosition.y) * yScale <= 0 ) )
		//{
		//	dir = 1;

		//	updateVelocity();
		//}
	}
}

void PlattformComponent::setPoints(int start, int end)
{
	if (start == -1)
		pos1 = startPosition;
	else
		pos1 = wayPoints[start];

	if (end == -1)
		pos2 = startPosition;
	else
		pos2 = wayPoints[end];

	xScale = 1;
	yScale = 1;

	if (pos1.x > pos2.x)
	{
		xScale = -1;
	}
	if (pos1.y > pos2.y)
	{
		yScale = -1;
	}

	parent->setPosition(pos1 + startValue * (pos2 - pos1));

	dirN = MATH::sfTob2(pos2 - parent->getPosition());
	dirN.Normalize();
	updateVelocity();
}

void PlattformComponent::updateVelocity()
{
	b2Vec2 v = dirN;
	v *= /*dir **/ velocity;
	v *= parent->getTimescale();

	getBody()->SetLinearVelocity(v);

}

//void PlattformComponent::preStep()
//{
//	if (stepCounter < 120)
//	{
//		b2Vec2 velocity(0,4);
//		velocity *= parent->getTimescale();
//		parent->getBody()->SetLinearVelocity(velocity);
//	}
//	else if (stepCounter < 240)
//	{
//		b2Vec2 velocity(0,-4);
//		velocity *= parent->getTimescale();
//
//		parent->getBody()->SetLinearVelocity(velocity);
//	}
//	else
//	{
//		stepCounter = 0;
//	}
//
//
//
//	stepCounter += parent->getTimescale();
//
//}
//
//void PlattformComponent::postStep()
//{
//
//
//
//
//}

void PlattformComponent::setVelocity(float _velocity)
{
	velocity = _velocity;
}

float PlattformComponent::getVelocity()
{
	return velocity;
}


void PlattformComponent::setStartPosition(sf::Vector2f _startPosition)
{
	startPosition = _startPosition;
}
//
//void PlattformComponent::setEndPosition(sf::Vector2f _endPosition)
//{
//	endPosition = _endPosition;
//}

void PlattformComponent::addWayPoint(sf::Vector2f wayPoint)
{
	wayPoints.push_back(wayPoint);
}

void PlattformComponent::deleteWayPoint()
{
	wayPoints.pop_back();
}


sf::Vector2f PlattformComponent::getWayPoint(int id)
{
	return wayPoints[id];
}


void PlattformComponent::setWayPoint(int id, sf::Vector2f wayPoint)
{
	wayPoints[id] = wayPoint;
}


float PlattformComponent::getNumWayPoints()
{
	return wayPoints.size();
}


//sf::Vector2f PlattformComponent::getEndPosition()
//{
//	return endPosition;
//}



void PlattformComponent::debugDraw(Camera* camera, sf::RenderTarget* target)
{
	//sf::Sprite sprite =  getRenderer()->getSprite();

	//sprite.setPosition(MATH::vMtoPx(endPosition));
	//sprite.setColor(sf::Color(0,0,255,100));


	//target->draw(sprite);

	//sprite.setPosition(MATH::vMtoPx(startPosition));

	//target->draw(sprite);

	if (wayPoints.size() <= 0)
		return;

	
	if (getRenderer())
	{
		getRenderer()->draw(wayPoints[0], 0.0f, camera, target, sf::Color(0,0,255,100));
		getRenderer()->draw(startPosition, 0.0f, camera, target, sf::Color(0,0,255,100));
	}

	sf::Vector2f diff(wayPoints[0] - startPosition);

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(MATH::magnitude(diff) * mToPx, 20.0f));
	rect.setOrigin(0.0f, 10.0f);


	rect.setPosition(MATH::vMtoPx(startPosition));
	rect.setRotation(-atan2(diff.y, diff.x) * 180/b2_pi);

	rect.setFillColor(sf::Color(0,0,255,100));

	target->draw(rect);
	
	for (int i = 0; i < wayPoints.size()-1; i++)
	{

		if (getRenderer())
		{
			getRenderer()->draw(wayPoints[i+1], 0.0f, camera, target, sf::Color(0,0,255,100));
			getRenderer()->draw(wayPoints[i], 0.0f, camera, target, sf::Color(0,0,255,100));
		}

		sf::Vector2f diff(wayPoints[i+1] - wayPoints[i]);

		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(MATH::magnitude(diff) * mToPx, 20.0f));
		rect.setOrigin(0.0f, 10.0f);


		rect.setPosition(MATH::vMtoPx(wayPoints[i]));
		rect.setRotation(-atan2(diff.y, diff.x) * 180/b2_pi);

		rect.setFillColor(sf::Color(0,0,255,100));

		target->draw(rect);
	}
}


void PlattformComponent::sendMessage(Message& msg)
{
	if (msg == "DEACTIVATE" || msg == "DEACTIVATE_AI")
	{
		if (getBody())
			getBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));

	}
}





void PlattformComponent::setStartValue(float value)
{
	startValue = value;
}

float PlattformComponent::getStartValue()
{
	return startValue;
}