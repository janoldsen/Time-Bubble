#include "Level.h"
#include "ImageManager.h"
#include "Prototype.h"



void Level::shutdown(Prototype* game)
{

	if (body)
	{
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
		{
			delete f->GetUserData();
			f->SetUserData(0);

		}

		game->world->DestroyBody(body);

		body = 0;
	}



	backGround.shutdown();
	gameGround.shutdown();

	//backGround.setDepth(16, false);

}


void Level::drawBackGround(Camera* camera, sf::RenderTarget* target, bool debug)
{
	backGround.draw(sf::Vector2f(size.left + size.width/2.0f, size.top - size.height/2.0f), 0.0f,  camera, target);
}

void Level::drawGameGround(Camera* camera, sf::RenderTarget* target, bool debug, sf::Color color)
{

	gameGround.draw(sf::Vector2f(size.left + size.width/2.0f, size.top - size.height/2.0f ), 0.0f, camera, target, color);

	if (debug && body)
	{
		b2Fixture* fix = body->GetFixtureList();

		for (; fix; fix = fix->GetNext())
		{
			camera->drawChain(dynamic_cast<b2ChainShape*>(fix->GetShape()), sf::Vector2f(0.0f, 0.0f));
		}

	}
}



void Level::setBackGround(sf::Texture& texture)
{

	backGround.setSprite(texture);
	backGround.initialize();

	backGround.setDepth(8.0f, false);

	bgPosition.y = texture.getSize().y / (2.0f * mToPx) + (size.top - size.height);

	//backGround.setTexture(texture);
	//backGround.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);

	
}

void Level::setBackGround(SpriteComponent& _backGround)
{
	backGround = _backGround;
	
	//bgPosition.y = backGround.getSprite().getTextureRect().height / (2.0f * mToPx) + (size.top - size.height);
}


const sf::Texture* Level::getBackGround()
{
	return backGround.getSprite().getTexture();
}


void Level::setGameGround(sf::Texture& texture)
{


	gameGround.setSprite(texture);
	gameGround.initialize();

	//gameGround.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
}

void Level::setGameGround(SpriteComponent& _gameGround)
{
	gameGround = _gameGround;
}

const sf::Texture* Level::getGameGround()
{
	return gameGround.getSprite().getTexture();
}

void Level::setBody(b2BodyDef* bodydef, b2World* world)
{
	body = world->CreateBody(bodydef);
}

b2Body* Level::getBody()
{
	return body;
}


void Level::setSize(sf::IntRect _size)
{
	size = _size;
}

const sf::IntRect& Level::getSize() const
{
	return size;
}


void Level::write(std::fstream& stream)
{
	//std::string bGFilename = imageManager->getFileName(backGround.getTexture());
	//int bGFilenameSize = bGFilename.size();

	//stream.write((char*) bGFilenameSize, sizeof(int));
	//stream.write(bGFilename.c_str(), bGFilenameSize);

	stream.write((char*)&bgPosition, sizeof(bgPosition));
	
	backGround.write(stream);

	//std::string gGFilename = imageManager->getFileName(gameGround.getTexture());
	//int gGFilenameSize = gGFilename.size();

	//stream.write((char*) gGFilenameSize, sizeof(int));
	//stream.write(gGFilename.c_str(), gGFilenameSize);

	gameGround.write(stream);

	writeBody(stream);

	stream.write((char*)&size, sizeof(size));

}

void Level::writeBody(std::fstream& stream)
{
	unsigned int numberOfFixtures = 0;

	b2Fixture* fix = body->GetFixtureList();
	for (; fix; fix = fix->GetNext())
		numberOfFixtures++;

	stream.write((char*)&numberOfFixtures, sizeof(numberOfFixtures));


	fix = body->GetFixtureList();
	for(; fix; fix = fix->GetNext())
	{
		b2ChainShape* chainShape = dynamic_cast<b2ChainShape*>(fix->GetShape());

		int vertexCount = chainShape->m_count;
		b2Vec2* vertices = chainShape->m_vertices;

		stream.write((char*)&vertexCount, sizeof(vertexCount));
		stream.write((char*)vertices, vertexCount * sizeof(b2Vec2));



		float32 friction	= fix->GetFriction();
		float32 restitution	= fix->GetRestitution();
		int		userData	= *((int*)fix->GetUserData());

		stream.write((char*)&friction, sizeof(friction));
		stream.write((char*)&restitution, sizeof(restitution));
		stream.write((char*)&userData, sizeof(userData));
	}


}

void Level::read(std::fstream& stream, Prototype* game, int versionNumber)
{
	//int bGFilenameSize;
	//char bGFilename[32];


	//stream.read((char*)&bGFilenameSize, sizeof(int));
	//stream.read(bGFilename, bGFilenameSize);

	//setBackGround(imageManager->getTexture(std::string(bGFilename, bGFilename + bGFilenameSize)));

	stream.read((char*)&bgPosition, sizeof(bgPosition));

	backGround.read(stream, versionNumber);

	
	//int gGFilenameSize;
	//char gGFilename[32];

	//stream.read((char*)&gGFilenameSize, sizeof(int));
	//stream.read(bGFilename, gGFilenameSize);

	//setGameGround(imageManager->getTexture(std::string(gGFilename, gGFilename + gGFilenameSize)));

	gameGround.read(stream, versionNumber);


	readBody(stream, game);

	stream.read((char*)&size, sizeof(size));

}


void Level::readBody(std::fstream& stream, Prototype* game)
{
	b2BodyDef bodyDef;

	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;

	setBody(&bodyDef, game->world);

	unsigned int numberOfFixtures;
	
	stream.read((char*)&numberOfFixtures, sizeof(numberOfFixtures));

	for (unsigned int i = 0; i < numberOfFixtures; i++)
	{
		b2FixtureDef fixtureDef;

		b2ChainShape shape;

		int vertexCount;
		b2Vec2* vertices;

		stream.read((char*)&vertexCount, sizeof(vertexCount));
		vertices = new b2Vec2[vertexCount];
		stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));

		shape.CreateChain(vertices, vertexCount);

		delete[] vertices;

		fixtureDef.shape = &shape;

		float32 friction;	
		float32 restitution;	
		int		userData;		
		
		stream.read((char*)&friction, sizeof(friction));
		stream.read((char*)&restitution, sizeof(restitution));
		stream.read((char*)&userData, sizeof(userData));
		
		fixtureDef.friction		= friction;
		fixtureDef.restitution	= restitution;
		fixtureDef.userData		= new int(userData);
		
		addFixture(&fixtureDef);

	}


}


void Level::addFixture(b2FixtureDef* fixtureDef)
{
	body->CreateFixture(fixtureDef);
}