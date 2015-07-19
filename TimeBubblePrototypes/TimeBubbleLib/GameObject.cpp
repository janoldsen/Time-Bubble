#include "GameObject.h"
#include "AiComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include <iostream>
#include "Prototype.h"
#include "myMath.h"
#include <cmath>
#include "AnimationComponent.h"
#include "PlattformComponent.h"
#include "PlayerComponent.h"
#include "JumperComponent.h"
#include "SwingComponent.h"
#include "TimeBubbleComponent.h"
#include "EventComponent.h"
#include "EnemyComponent.h"
#include "ShroomComponent.h"
#include "ParticleComponent.h"
#include "RockComponent.h"
#include "BombComponent.h"
//GameObject::GameObject()
//{
//	body	= NULL;
//	renderer = NULL;
//	ai = NULL;
//
//	angle = 0.0f;
//
//	center = sf::Vector2f(0.0,0.0);
//
//	bodyIsActive = true;
//	aiIsActive	= true;
//
//	orientation = RIGHT;
//
//	clone = false;
//
//}


GameObject::GameObject(std::string _name)
{

	body	= NULL;
	renderer = NULL;
	ai = NULL;

	angle = 0.0f;

	center = sf::Vector2f(0.0,0.0);

	bodyIsActive = true;
	aiIsActive	= true;

	orientation = RIGHT;

	clone = false;
	name = _name;


}



void GameObject::write(std::fstream& stream)
{

	int nameSize = name.size();
	stream.write((char*)&nameSize, sizeof(nameSize));
	stream.write(name.c_str(), nameSize);

	stream.write((char*)&position, sizeof(position));
	stream.write((char*)&center, sizeof(center));
	stream.write((char*)&orientation, sizeof(orientation));


	writeBody(stream);
	
	writeRenderer(stream);

	writeAi(stream);

}


void GameObject::writeBody(std::fstream& stream)
{
	ComponentTypes bodyType;
	if (body)
	{
		bodyType = ComponentTypes::STANDARD;
		stream.write((char*)&bodyType, sizeof(bodyType));

		b2BodyType	type			= body->GetType();
		b2Vec2		position		= body->GetPosition();
		float32		angle			= body->GetAngle();
		bool		fixedRotation	= body->IsFixedRotation();
		bool		bullet			= body->IsBullet();
		float		gravityScale	= body->GetGravityScale();

		bool		active			= body->IsActive();

		stream.write((char*)&type, sizeof(type));
		stream.write((char*)&position, sizeof(position));
		stream.write((char*)&angle, sizeof(angle));
		stream.write((char*)&fixedRotation, sizeof(fixedRotation));
		stream.write((char*)&bullet, sizeof(bullet));
		stream.write((char*)&gravityScale, sizeof(gravityScale));

		stream.write((char*)&active, sizeof(active));

		unsigned int numberOfFixtures = 0;

		b2Fixture* fix = body->GetFixtureList();
		for (; fix; fix = fix->GetNext())
			numberOfFixtures++;

		stream.write((char*)&numberOfFixtures, sizeof(numberOfFixtures));

		fix = body->GetFixtureList();
		for(; fix; fix = fix->GetNext())
		{

			b2Shape::Type shapeType = fix->GetType();
			stream.write((char*)&shapeType, sizeof(shapeType));

			switch (shapeType) // write shape
			{
			case b2Shape::Type::e_polygon:
				{
					b2PolygonShape* polyShape = dynamic_cast<b2PolygonShape*>(fix->GetShape());
				
					int vertexCount = polyShape->GetVertexCount();
					b2Vec2* vertices = polyShape->m_vertices;

					stream.write((char*)&vertexCount, sizeof(vertexCount));
					stream.write((char*)vertices, vertexCount * sizeof(b2Vec2));
				}
				break;

			case b2Shape::Type::e_circle:
				{
					b2CircleShape* circleShape = dynamic_cast<b2CircleShape*>(fix->GetShape());

					float32 radius	= circleShape->m_radius;
					b2Vec2 position = circleShape->m_p;

					stream.write((char*)&radius, sizeof(radius));
					stream.write((char*)&position, sizeof(position));
				}
				break;

			case b2Shape::Type::e_chain:
				{
					b2ChainShape* chainShape = dynamic_cast<b2ChainShape*>(fix->GetShape());

					int vertexCount = chainShape->m_count;
					b2Vec2* vertices = chainShape->m_vertices;

					stream.write((char*)&vertexCount, sizeof(vertexCount));
					stream.write((char*)vertices, vertexCount * sizeof(b2Vec2));

				}
				break;
			}

			float32 friction	= fix->GetFriction();
			float32 density		= fix->GetDensity();
			float32 restitution	= fix->GetRestitution();
			int		userData	= *((int*)fix->GetUserData());
			b2Filter filter		= fix->GetFilterData();
			bool isSensor		= fix->IsSensor();

			stream.write((char*)&friction, sizeof(friction));
			stream.write((char*)&density, sizeof(density));
			stream.write((char*)&restitution, sizeof(restitution));
			stream.write((char*)&userData, sizeof(userData));
			stream.write((char*)&filter, sizeof(filter));
			stream.write((char*)&isSensor, sizeof(isSensor));

		}



	}
	else
	{
		bodyType = ComponentTypes::ZERO;
		stream.write((char*)&bodyType, sizeof(bodyType));
	}
	
}

void GameObject::writeRenderer(std::fstream& stream)
{
	ComponentTypes rendererType;

	if (renderer)
	{
		rendererType = renderer->getType();
		stream.write((char*)&rendererType, sizeof(rendererType));
		renderer->write(stream);
	}
	else
	{
		rendererType = ComponentTypes::ZERO;
		stream.write((char*)&rendererType, sizeof(rendererType));
	}
}

void GameObject::writeAi(std::fstream& stream)
{
	ComponentTypes aiType;

	if (ai)
	{
		aiType = ai->getType();
		stream.write((char*)&aiType, sizeof(aiType));
		ai->write(stream);
	}
	else
	{
		aiType = ComponentTypes::ZERO;
		stream.write((char*)&aiType, sizeof(aiType));
	}



}

void GameObject::read(std::fstream& stream, Prototype* _game, int versionNumber)
{

	int nameSize;
	//char buffer[32];

	stream.read((char*)&nameSize, sizeof(nameSize));

	name = std::string(nameSize, '\0');
	stream.read(&name[0], sizeof(char) * nameSize);
		//stream.read(buffer, nameSize);
	//name = std::string(buffer, buffer+nameSize);


	stream.read((char*)&position, sizeof(position));
	stream.read((char*)&center, sizeof(center));
	stream.read((char*)&orientation, sizeof(orientation));


	readBody(stream, _game, versionNumber);

	readRenderer(stream, versionNumber);

	readAi(stream, versionNumber);


}

void GameObject::readBody(std::fstream& stream, Prototype* _game, int versionNumber)
{
	ComponentTypes bodyType = ComponentTypes::ZERO;

	stream.read((char*)&bodyType, sizeof(ComponentTypes));

	if (bodyType == ComponentTypes::STANDARD)
	{
		b2BodyType	type;
		b2Vec2		position;
		float32		angle;
		bool		fixedRotation;
		bool		bullet;
		float		gravityScale;
		bool		active;

		stream.read((char*)&type, sizeof(type));
		stream.read((char*)&position, sizeof(position));
		stream.read((char*)&angle, sizeof(angle));
		stream.read((char*)&fixedRotation, sizeof(fixedRotation));
		stream.read((char*)&bullet, sizeof(bullet));
		stream.read((char*)&gravityScale, sizeof(gravityScale));

		if (versionNumber >= 5)
			stream.read((char*)&active, sizeof(active));



		b2BodyDef bodyDef;

		bodyDef.type = type;
		bodyDef.position = position;
		bodyDef.angle = angle;
		bodyDef.fixedRotation = fixedRotation;
		bodyDef.bullet = bullet;
		bodyDef.gravityScale = gravityScale;
		if (versionNumber >= 5)
			bodyDef.active = active;

		setBody(&bodyDef, _game->world);
		//game->world->CreateBody(&bodyDef);


		unsigned int numberOfFixtures;

		stream.read((char*)&numberOfFixtures, sizeof(numberOfFixtures));




		for (unsigned int i = 0; i < numberOfFixtures; i++)
		{
			
			b2FixtureDef fixtureDef;
			b2Shape::Type shapeType;

			stream.read((char*)&shapeType, sizeof(shapeType));



			b2PolygonShape polyShape;
			b2CircleShape circleShape;
			b2ChainShape chainShape;

			switch (shapeType) // read shape
			{
			case b2Shape::Type::e_polygon:
				{
					if (versionNumber < 3)
					{
						int vertexCount;
						b2Vec2* vertices;


						stream.read((char*)&vertexCount, sizeof(vertexCount));
						vertices = new b2Vec2[vertexCount];
						stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));

						polyShape.Set(vertices, vertexCount);

						delete[] vertices;

						fixtureDef.shape = &polyShape;
					}
					else
					{
						b2PolygonShape _polyShape;
						stream.read((char*)&_polyShape, sizeof(_polyShape));

						polyShape = _polyShape;

						fixtureDef.shape = &polyShape;

					}

				}
				break;
				
			case b2Shape::Type::e_circle:
				{
					
					//b2CircleShape circleShape;
					if (versionNumber < 3)
					{
						float32 radius;
						b2Vec2	position;

						stream.read((char*)&radius, sizeof(radius));
						stream.read((char*)&position, sizeof(position));

						circleShape.m_radius = radius;
						circleShape.m_p = position;

						fixtureDef.shape = &circleShape;
					}
					else
					{
						b2CircleShape _circleShape;

						stream.read((char*)&_circleShape, sizeof(_circleShape));

						circleShape.m_radius = _circleShape.m_radius;
						circleShape.m_p = _circleShape.m_p;

						//stream.read((char*)&circleShape, sizeof(circleShape));

						fixtureDef.shape = &circleShape;
					}
				}
				break;
				
			case b2Shape::Type::e_chain:
				{
					//b2ChainShape chainShape;
					int vertexCount;
					b2Vec2* vertices;

					stream.read((char*)&vertexCount, sizeof(vertexCount));
					vertices = new b2Vec2[vertexCount];
					stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));

					chainShape.CreateChain(vertices, vertexCount);

					delete[] vertices;

					fixtureDef.shape = &chainShape;

				}
				break;

			}

			float32		friction;
			float32		density = 0.0f;
			float32		restitution;	
			int			userData;		
			b2Filter	filter;
			bool		sensor;

			stream.read((char*)&friction, sizeof(friction));
			if (versionNumber >= 4)
				stream.read((char*)&density, sizeof(density));
			stream.read((char*)&restitution, sizeof(restitution));
			stream.read((char*)&userData, sizeof(userData));
			stream.read((char*)&filter, sizeof(filter));
			stream.read((char*)&sensor, sizeof(sensor));

			fixtureDef.friction		= friction;
			fixtureDef.density		= density;
			fixtureDef.restitution	= restitution;
			fixtureDef.userData		= new int(userData);
			fixtureDef.filter		= filter;
			fixtureDef.isSensor		= sensor;

			addFixture(&fixtureDef);

		}
	}
}

void GameObject::readRenderer(std::fstream& stream, int versionNumber)
{
	ComponentTypes rendererType;
	stream.read((char*)&rendererType, sizeof(rendererType));

	switch (rendererType)
	{
	case ComponentTypes::STANDARD:
		renderer = new SpriteComponent();
		break;
	case ComponentTypes::ANIMATION:
		renderer = new AnimationComponent();
		break;
	case ComponentTypes::SCML:
		renderer = new SCMLComponent();
		break;
	}
	
	if (renderer)
		renderer->read(stream, versionNumber);

}

void GameObject::readAi(std::fstream& stream, int versionNumber)
{
	ComponentTypes aiType;
	stream.read((char*)&aiType, sizeof(aiType));

	switch (aiType)
	{
	case ComponentTypes::PLAYER:
		ai = new PlayerComponent();
		break;
	case ComponentTypes::JUMPER:
		ai = new JumperComponent();
		break;
	case ComponentTypes::PLATTFORM:
		ai = new PlattformComponent();
		break;
	case ComponentTypes::SWING:
		ai = new SwingComponent();
		break;
	case ComponentTypes::BUBBLE:
		ai = new TimeBubbleComponent();
		break;
	case ComponentTypes::EVENT:
		ai = new EventComponent();
		break;
	case ComponentTypes::ENEMY:
		ai = new EnemyComponent();
		break;
	case ComponentTypes::SHROOM:
		ai = new ShroomComponent();
		break;
	case ComponentTypes::PARTICLE:
		ai = new ParticleComponent();
		break;
	case ComponentTypes::ROCK:
		ai = new RockComponent();
		break;
	case ComponentTypes::BOMB:
		ai = new BombComponent();
		break;
	}

	if (ai)
		ai->read(stream, versionNumber);
}

void GameObject::initialize(Prototype* _game)
{

	game = _game;


	if (orientation != 3 && orientation != 1)
		orientation = RIGHT;

	if (body)
	{
		body->SetUserData(this);
		bodyIsActive = body->IsActive();
	}

	if (renderer)
	{
		renderer->initialize();
		renderer->setOrientation(orientation);
	}
	if (ai)
		ai->initialize(this);

	state = NORMAL;
	nextState = state;



}

void GameObject::shutdown()
{



	if (body)
	{
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
		{
			delete f->GetUserData();
			f->SetUserData(0);

		}
		//for (b2Fixture* f = fastBody->GetFixtureList(); f; f = f->GetNext())
		//	delete f->GetUserData();
		//for (b2Fixture* f = slowBody->GetFixtureList(); f; f = f->GetNext())
		//	delete f->GetUserData();

		game->world->DestroyBody(body);
		//game->slowWorld->DestroyBody(slowBody);
		//game->fastWorld->DestroyBody(fastBody);

		body = 0;
	}



	if (renderer)
	{
		renderer->shutdown();
		delete renderer;

		renderer = 0;
	}
	if (ai)
	{
		ai->shutdown();
		delete ai;

		ai = 0;
	}

}

void GameObject::update()
{
	if (body)
	{
		if (bodyIsActive != body->IsActive())
			body->SetActive(bodyIsActive);

	}

	if (nextState != state)
		setState(nextState);





	if (clone)
		doClone();

	if (ai && aiIsActive)
		ai->update();

	if (renderer)
		renderer->update(getTimescale());

}


void GameObject::draw(Camera* camera, sf::RenderTarget* target, bool debug, sf::Color color)
{
	if (renderer)
		renderer->draw(getPosition(), getAngle() , camera, target, color);

	if (ai)
		ai->draw(camera, target);

	if (debug)
	{
		if (body)
		{
			b2Fixture* fix = body->GetFixtureList();


			for (; fix; fix = fix->GetNext())
			{
				switch (fix->GetType())
				{
				case b2Shape::e_polygon:
					camera->drawPolygon(dynamic_cast<b2PolygonShape*>(fix->GetShape()), getPosition(), getAngle());
					break;
				case b2Shape::e_chain:
					camera->drawChain(dynamic_cast<b2ChainShape*>(fix->GetShape()), getPosition(), getAngle());
					break;
				case b2Shape::e_circle:
					camera->drawCircle(dynamic_cast<b2CircleShape*>(fix->GetShape()), getPosition(), getAngle());
					break;
				}


			}

		}

		if (ai)
			ai->debugDraw(camera, target);

		//center
		sf::CircleShape centerCircle;
		centerCircle.setFillColor(sf::Color::Red);
		centerCircle.setRadius(7.0f);
		centerCircle.setOrigin(7.0f,7.0f);
		centerCircle.setPosition(getCenter().x * mToPx, getCenter().y * -mToPx);

		target->draw(centerCircle);

	}


}


void GameObject::setName(std::string _name)
{
	name = _name;
}

std::string GameObject::getName()
{
	return name;
}


sf::Vector2f GameObject::getPosition()
{
	
	if (body)
	{
		return MATH::b2ToSf(body->GetPosition());
	}
	else
	{
		return position;
	}
}


void GameObject::setPosition(sf::Vector2f _position)
{
	if (body)
	{
		b2Vec2 position(_position.x, _position.y);
		body->SetTransform(position, body->GetAngle());

	}
	else
	{
		position = _position;
	}
}


void GameObject::setCenter(sf::Vector2f _center)
{
	center = _center;
}


sf::Vector2f GameObject::getCenter()
{
	float _angle = getAngle();
	

	sf::Vector2f _center( cos(_angle) * center.x - sin(_angle) * center.y,
		cos(_angle) * center.y + sin(_angle) * center.x);

	return getPosition() + _center;
}

float GameObject::getAngle()
{
	if (body)
	{
		return body->GetAngle();
	}
	else
	{
		return angle;
	}
}


void GameObject::setAngle(float _angle)
{
	if (body)
	{
		body->SetTransform(body->GetWorldCenter(), _angle);
	}
	else
	{
		angle = _angle;
	}

}

void GameObject::setOrientation(Orientation _orientation)
{
	if (renderer)
		renderer->setOrientation(_orientation);

	orientation = _orientation;

}


Orientation GameObject::getOrientation()
{
	return orientation;
}


b2Body* GameObject::setBody(b2BodyDef* bodyDef, b2World* world)
{
	return body = world->CreateBody(bodyDef);
}

const b2Body* GameObject::getBody() const
{
	return body;
}


b2Fixture* GameObject::addFixture(b2FixtureDef* fixtureDef)
{
	return body->CreateFixture(fixtureDef);
}




void GameObject::setRendererComponent(RendererComponent* rendererComponent)
{
	renderer = rendererComponent;
}

void GameObject::setAiComponent(AiComponent* aiComponent)
{
	ai = aiComponent;
}

//AiComponent* GameObject::getAiComponent()
//{
//	return ai;
//}


void GameObject::sendMessage(Message& msg)
{
	if (msg == "ACTIVATE")
	{
		bodyIsActive = true;
		aiIsActive =  true;

	}
	else if (msg == "DEACTIVATE")
	{
		bodyIsActive = false;
		aiIsActive =  false;
	}
	else if (msg == "TOGGLE")
	{
		bodyIsActive = !bodyIsActive;
		aiIsActive = !aiIsActive;
	}
	else if (msg == "ACTIVATE_PHYSICS")
	{
		bodyIsActive = true;
	}
	else if (msg == "DEACTIVATE_PHYSICS")
	{
		bodyIsActive = false;
	}
	else if (msg == "TOGGLE_PHYSICS")
	{
		bodyIsActive != bodyIsActive;
	}
	else if (msg == "ACTIVATE_AI")
	{
		aiIsActive =  true;
	}
	else if (msg == "DEACTIVATE_AI")
	{
		aiIsActive = false;
	}
	else if (msg == "TOGGLE_AI")
	{
		aiIsActive != aiIsActive;
	}
	else if (msg == "CLONE_TO")
	{
		clone = true;

		clonePosition.x = *(float*)msg.getArgument(0);
		clonePosition.y = *(float*)msg.getArgument(1);
		cloneAngle		= *(float*)msg.getArgument(2);

	}
	else if (msg == "PLAY_ANIMATION")
	{
		renderer->sendMessage(msg);
	}
	else if (msg == "COUT")
	{
		std::cout <<
			*(float*)msg.getArgument(0) <<
			*(float*)msg.getArgument(1) <<
			*(float*)msg.getArgument(2) <<
			*(float*)msg.getArgument(3) << std::endl;

	}
	else if (msg == "ADD_IMPULSE")
	{
		if (body)
		{
			b2Vec2 imp = b2Vec2(*(float*)msg.getArgument(0), *(float*)msg.getArgument(1));
			imp *= getTimescale();

			body->ApplyLinearImpulse(imp, body->GetWorldCenter(), true);
			
		}
	}



	if (ai)
		ai->sendMessage(msg);
}



void GameObject::setState(State _state)
{
	if (state == _state)
		return;

	if (!body)
		return;

	b2Vec2 velocity = body->GetLinearVelocity();

	switch (_state)
	{

	case NORMAL:

		if (state == SLOW)
		{
			velocity *= 2.0f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 2.0f);
		}
		if (state == FAST)
		{
			velocity *= 0.5f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 0.5f);

		}

		

		state = NORMAL;
		break;

	case SLOW:

		if (state == FAST)
		{
			velocity *= 0.25f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 0.25f);
		}
		if (state == NORMAL)
		{
			velocity *= 0.5f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 0.5f);

		}

		//body->SetGravityScale(0.5f);

		state = SLOW;
		break;

	case FAST:
		
		if (state == SLOW)
		{
			velocity *= 4.0f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 4.0f);
		}
		if (state == NORMAL)
		{
			velocity *= 2.0f;
			body->SetLinearVelocity(velocity);
			body->SetGravityScale(body->GetGravityScale() * 2.0f);

		}

		//body->SetGravityScale(2.0f);

		state = FAST;
		break;
	
	}

	if (ai && aiIsActive)
		ai->onStateChange();

}


void GameObject::setNextState(State _state)
{

	nextState = _state;
}

State GameObject::getState()
{
	return state;
}

float GameObject::getTimescale()
{
	switch (state)
	{
	case NORMAL:
		return 1.0f;
	case SLOW:
		return 0.5f;
	case FAST:
		return 2.0f;
	default:
		return 1.0f;
	}

}




bool GameObject::testPoint(b2Vec2 _point)
{
	b2Fixture* fix = body->GetFixtureList();

	for (; fix; fix = fix->GetNext())
	{
		if (fix->TestPoint(_point))
			return true;
	}

	return false;


}


void GameObject::setActive(bool flag)
{
	aiIsActive = flag;
	bodyIsActive = flag;
}

void GameObject::doClone()
{
	GameObject* _clone = new GameObject();

	if (body)
	{
		b2BodyDef bodyDef;

		bodyDef.active = body->IsActive();
		bodyDef.angularVelocity = body->GetAngularVelocity();
		bodyDef.bullet = body->IsBullet();
		bodyDef.fixedRotation = body->IsFixedRotation();
		bodyDef.gravityScale = body->GetGravityScale();
		bodyDef.linearVelocity = body->GetLinearVelocity();
		bodyDef.type = body->GetType();
		bodyDef.userData = _clone;

		_clone->setBody(&bodyDef, game->world);

		for(b2Fixture* fix = body->GetFixtureList(); fix; fix = fix->GetNext())
		{
			b2FixtureDef fixtureDef;

			fixtureDef.density = fix->GetDensity();
			fixtureDef.filter = fix->GetFilterData();
			fixtureDef.friction = fix->GetFriction();
			fixtureDef.isSensor = fix->IsSensor();
			fixtureDef.restitution = fix->GetRestitution();
			fixtureDef.shape = fix->GetShape();
			int* userData = new int(*(int*)fix->GetUserData());
			fixtureDef.userData = userData;

			_clone->addFixture(&fixtureDef);

		}

	}

	if (renderer)
		_clone->setRendererComponent(renderer->clone());

	if (ai)
		_clone->setAiComponent(ai->clone());

	_clone->setPosition(clonePosition);
	_clone->setAngle(cloneAngle);

	_clone->initialize(game);
	game->addObject(_clone);

	clone = false;
}