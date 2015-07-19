#include "LevelObject.h"
//#include <myMath.h>
#include "RendererComponent.h"
#include "AiComponent.h"
#include "SelectionObject.h"
#include <myMath.h>
#include "LevelEditor.h"

LevelObject::LevelObject()
{
	hasBody = false;
}






void LevelObject::draw(Camera* camera, sf::RenderTarget* target, bool debug, sf::Color color)
{
	
	float _angle = angle;

	if (hasBody)
		_angle = bodyDef.angle;

	if (renderer)
		renderer->draw(position, _angle, camera, target, color);


	if (debug)
	{
		if (hasBody)
		{
			for (int i = 0; i < shapes.size(); i++)
			{
				switch (shapes[i]->GetType())
				{
				case b2Shape::e_polygon:
					camera->drawPolygon(dynamic_cast<b2PolygonShape*>(shapes[i]), position, _angle);
					break;
				case b2Shape::e_chain:
					camera->drawChain(dynamic_cast<b2ChainShape*>(shapes[i]), position, _angle);
					break;
				case b2Shape::e_circle:
					camera->drawCircle(dynamic_cast<b2CircleShape*>(shapes[i]), position, _angle);
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
		centerCircle.setPosition((getPosition().x + center.x) * mToPx, (getPosition().y + center.y) * -mToPx);

		target->draw(centerCircle);
	}

}




RendererComponent* LevelObject::getRendererComponent()
{
	return renderer;
}

AiComponent* LevelObject::getAiComponent()
{
	return ai;
}


void LevelObject::exportObject()
{
	std::fstream out;

	out.open("assets\\" + name + ".ass", std::ios::out | std::ios::trunc | std::ios::binary);

	if (VERSION >= 3)
	{
		int versionNumber = VERSION;
		out.write((char*)&versionNumber, sizeof(versionNumber));
	}

	write(out);

	out.close();
}

void LevelObject::importObject(std::string filename)
{
	std::fstream in;

	in.open("assets\\" + filename, std::ios::in | std::ios::binary);


	int versionNumber = 0;
	if (VERSION >= 3)
	{
		in.read((char*)&versionNumber, sizeof(versionNumber));
	}
	

	read(in, 0, versionNumber);

	in.close();


}



void LevelObject::setName(std::string _name)
{
	
	GameObject::setName(_name);
	if (selection.isBuilt())
		selection.updateHierarchyButton();
}


GameObject* LevelObject::getGameObject(Prototype* prototype)
{
	GameObject* gameObject = new GameObject();


	gameObject->setName(name);
	gameObject->setPosition(position);
	gameObject->setCenter(center);
	gameObject->setOrientation(orientation);


	if (hasBody)
	{
		bodyDef.position.x = position.x;
		bodyDef.position.y = position.y;

		gameObject->setBody(&bodyDef, prototype->world);
		
	
		for(int i = 0; i < fixtureDefs.size(); i++)
		{
			int* userData = (int*)fixtureDefs[i].userData;

			fixtureDefs[i].userData = new int(*userData);
			gameObject->addFixture(&fixtureDefs[i]);

			fixtureDefs[i].userData = userData;

		}

	}


	//RendererComponent* test = renderer->clone();

	if (renderer)
		gameObject->setRendererComponent(renderer->clone());
	if (ai)
		gameObject->setAiComponent(ai->clone());

	return gameObject;

}


void LevelObject::initialize()
{
	if (ai)
		ai->setParent(this);
}

void LevelObject::shutdown()
{
	for (int i = 0; i < fixtureDefs.size(); i++)
	{
		delete shapes[i];
		delete fixtureDefs[i].userData;
	}

	shapes.clear();
	fixtureDefs.clear();

	selection.shutdown();

	GameObject::shutdown();

}





void LevelObject::writeBody(std::fstream& stream)
{
	ComponentTypes bodyType;

	if (hasBody)
	{
		bodyType = ComponentTypes::STANDARD;

		b2Vec2 bodyPosition = MATH::sfTob2(position);

		stream.write((char*)&bodyType, sizeof(bodyType));

		stream.write((char*)&bodyDef.type, sizeof(bodyDef.type));
		stream.write((char*)&bodyPosition, sizeof(bodyPosition));
		stream.write((char*)&bodyDef.angle, sizeof(bodyDef.angle));
		stream.write((char*)&bodyDef.fixedRotation, sizeof(bodyDef.fixedRotation));
		stream.write((char*)&bodyDef.bullet, sizeof(bodyDef.bullet));
		stream.write((char*)&bodyDef.gravityScale, sizeof(bodyDef.gravityScale));

		stream.write((char*)&bodyDef.active, sizeof(bodyDef.active));

		unsigned int numberOfFixtures = fixtureDefs.size();

		stream.write((char*)&numberOfFixtures, sizeof(numberOfFixtures));

		for (int i = 0; i < numberOfFixtures; i++)
		{
			b2Shape::Type shapeType = fixtureDefs[i].shape->GetType();
			stream.write((char*)&shapeType, sizeof(shapeType));


			b2Shape* shape = shapes[i];

			switch (shapeType) // write shape
			{
			case b2Shape::Type::e_polygon:
				{
					b2PolygonShape* polyShape = dynamic_cast<b2PolygonShape*>(shape);

					//int vertexCount = polyShape->GetVertexCount();
					//b2Vec2* vertices = polyShape->m_vertices;

					//stream.write((char*)&vertexCount, sizeof(vertexCount));
					//stream.write((char*)vertices, vertexCount * sizeof(b2Vec2));

					stream.write((char*) polyShape, sizeof(*polyShape));

				}
				break;

			case b2Shape::Type::e_circle:
				{
					b2CircleShape* circleShape = dynamic_cast<b2CircleShape*>(shape);

					//float32 radius	= circleShape->m_radius;
					//b2Vec2 position = circleShape->m_p;

					//stream.write((char*)&radius, sizeof(radius));
					//stream.write((char*)&position, sizeof(position));

					stream.write((char*)circleShape, sizeof(*circleShape));

				}
				break;

			case b2Shape::Type::e_chain:
				{
					b2ChainShape* chainShape = dynamic_cast<b2ChainShape*>(shape);

					int vertexCount = chainShape->m_count;
					b2Vec2* vertices = chainShape->m_vertices;

					stream.write((char*)&vertexCount, sizeof(vertexCount));
					stream.write((char*)vertices, vertexCount * sizeof(b2Vec2));

				}
				break;
			}

			stream.write((char*)&fixtureDefs[i].friction, sizeof(fixtureDefs[i].friction));
			stream.write((char*)&fixtureDefs[i].density, sizeof(fixtureDefs[i].density));
			stream.write((char*)&fixtureDefs[i].restitution, sizeof(fixtureDefs[i].restitution));
			stream.write((char*)fixtureDefs[i].userData, sizeof(int));
			stream.write((char*)&fixtureDefs[i].filter, sizeof(fixtureDefs[i].filter));
			stream.write((char*)&fixtureDefs[i].isSensor, sizeof(fixtureDefs[i].isSensor));

		}

	}
	else
	{
		bodyType = ComponentTypes::ZERO;
		stream.write((char*)&bodyType, sizeof(bodyType));
	}



}



void LevelObject::readBody(std::fstream& stream, Prototype* _game, int versionNumber)
{

	ComponentTypes bodyType;

	stream.read((char*)&bodyType, sizeof(bodyType));

	if (bodyType == ComponentTypes::STANDARD)
	{

		hasBody = true;

		stream.read((char*)&bodyDef.type, sizeof(bodyDef.type));
		stream.read((char*)&bodyDef.position, sizeof(bodyDef.position));
		stream.read((char*)&bodyDef.angle, sizeof(bodyDef.angle));
		stream.read((char*)&bodyDef.fixedRotation, sizeof(bodyDef.fixedRotation));
		stream.read((char*)&bodyDef.bullet, sizeof(bodyDef.bullet));
		stream.read((char*)&bodyDef.gravityScale, sizeof(bodyDef.gravityScale));

		if (versionNumber >= 5)
			stream.read((char*)&bodyDef.active, sizeof(bodyDef.active));


		unsigned int numberOfFixtures;

		stream.read((char*)&numberOfFixtures, sizeof(numberOfFixtures));

		for (int i = 0; i < numberOfFixtures; i++)
		{
			b2FixtureDef fixtureDef;
			b2Shape::Type shapeType;

			stream.read((char*)&shapeType, sizeof(shapeType));

			//b2PolygonShape polyShape;
			//b2CircleShape circleShape;
			//b2ChainShape chainShape;


			switch(shapeType)
			{
				case b2Shape::Type::e_polygon:
				{

					if (versionNumber < 3)
					{

						b2PolygonShape* polyShape = new b2PolygonShape;
						int vertexCount;
						b2Vec2* vertices;


						stream.read((char*)&vertexCount, sizeof(vertexCount));
						vertices = new b2Vec2[vertexCount];
						stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));
						
						polyShape->Set(vertices, vertexCount);

						delete[] vertices;

						shapes.push_back(polyShape);
						fixtureDef.shape = polyShape;

					}
					else
					{
						b2PolygonShape polyShape;

						stream.read((char*)&polyShape, sizeof(polyShape));

						shapes.push_back(new b2PolygonShape(polyShape));
						fixtureDef.shape = shapes.back();

					}
				}
				break;
				
			case b2Shape::Type::e_circle:
				{

					if (versionNumber < 3)
					{

						b2CircleShape* circleShape = new b2CircleShape();
						float32 radius;
						b2Vec2	position;

						stream.read((char*)&radius, sizeof(radius));
						stream.read((char*)&position, sizeof(position));

						circleShape->m_radius = radius;
						circleShape->m_p = position;


						shapes.push_back(circleShape);
						fixtureDef.shape = circleShape;
					}
					else
					{
						b2CircleShape circleShape;

						stream.read((char*)&circleShape, sizeof(circleShape));

						shapes.push_back(new b2CircleShape(circleShape));
						fixtureDef.shape = shapes.back();

					}
				}
				break;
				
			case b2Shape::Type::e_chain:
				{
					b2ChainShape* chainShape = new b2ChainShape;
					int vertexCount;
					b2Vec2* vertices;

					stream.read((char*)&vertexCount, sizeof(vertexCount));
					vertices = new b2Vec2[vertexCount];
					stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));

					chainShape->CreateChain(vertices, vertexCount);

					delete[] vertices;


					shapes.push_back(chainShape);
					fixtureDef.shape = chainShape;

				}
				break;

			}

			stream.read((char*)&fixtureDef.friction, sizeof(fixtureDef.friction));
			if (versionNumber >= 4)
				stream.read((char*)&fixtureDef.density, sizeof(fixtureDef.density));
			stream.read((char*)&fixtureDef.restitution, sizeof(fixtureDef.restitution));
			int userData;
			stream.read((char*)&userData, sizeof(userData));
			stream.read((char*)&fixtureDef.filter, sizeof(fixtureDef.filter));
			stream.read((char*)&fixtureDef.isSensor, sizeof(fixtureDef.isSensor));
			
			fixtureDef.userData = new int(userData);

			fixtureDefs.push_back(fixtureDef);



		}


	}
	else
	{
		hasBody = false;
	}




}


void LevelObject::checkSelection(sf::FloatRect& selectionRect/*, std::vector<Selection*>& selection*/)
{
	if (selectionRect.contains(position))
	{
		//selection.push_back(new SelectionObject(this, &position));

		selection.select(true);
	}
}


void LevelObject::setSelection(LevelEditor* editor, Selector* selector, sfg::Box::Ptr* hierarchy)
{
	selection.set(editor, this, &position, selector, hierarchy);

}


