#include "ParticleComponent.h"
#include "GameClock.h"
#include "myMath.h"
#include "Prototype.h"
#include "ParticleManager.h"
#include "ImageManager.h"
#include <fstream>
#include "Camera.h"

ParticleComponent::ParticleComponent()
{
	particles = 0;
	maxParticles = 2000;
	count = 0.0f;
	depth = 1.0f;
	spawnIntervall = 1.0f;
	spawnTimer = 0.0f;
	spawnRadius = sf::Vector3f(10.0f, 10.0f, 10.0f);
	spawnCount = 10;
	minLifeTime = 0.5f;
	maxLifeTime = 1.0f;
	minVelocity = sf::Vector3f(-10.0f, -10.0f, -10.0f);
	maxVelocity = sf::Vector3f(10.0f, 10.0f, 10.0f);
	fieldScale = sf::Vector3f(0.1f, 0.1f, 0.1f);
	timeScale = 0.1f;
	scale = 1.0f;
}



void ParticleComponent::write(std::fstream& stream)
{
	stream.write((char*)&depth, sizeof(depth));
	stream.write((char*)&maxParticles, sizeof(maxParticles));
	stream.write((char*)&spawnIntervall, sizeof(spawnIntervall));
	stream.write((char*)&spawnRadius, sizeof(spawnRadius));
	stream.write((char*)&spawnCount, sizeof(spawnCount));

	std::string filename = imageManager->getFileName(sprite.getTexture());

	int size = filename.length();

	stream.write((char*)&size, sizeof(size));
	stream.write(filename.c_str(), size);

	stream.write((char*)&minLifeTime, sizeof(minLifeTime));
	stream.write((char*)&maxLifeTime, sizeof(maxLifeTime));
	stream.write((char*)&minVelocity, sizeof(minVelocity));
	stream.write((char*)&maxVelocity, sizeof(maxVelocity));
	stream.write((char*)&fieldScale, sizeof(fieldScale));
	stream.write((char*)&timeScale, sizeof(timeScale));

	stream.write((char*)&scale, sizeof(scale));
}

void ParticleComponent::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&depth, sizeof(depth));
	stream.read((char*)&maxParticles, sizeof(maxParticles));
	stream.read((char*)&spawnIntervall, sizeof(spawnIntervall));
	stream.read((char*)&spawnRadius, sizeof(spawnRadius));
	stream.read((char*)&spawnCount, sizeof(spawnCount));

	int size;

	stream.read((char*)&size, sizeof(size));
	std::string filepath(size, '\0');
	stream.read(&filepath[0], size * sizeof(char));
	setSprite(filepath);

	stream.read((char*)&minLifeTime, sizeof(minLifeTime));
	stream.read((char*)&maxLifeTime, sizeof(maxLifeTime));
	stream.read((char*)&minVelocity, sizeof(minVelocity));
	stream.read((char*)&maxVelocity, sizeof(maxVelocity));
	stream.read((char*)&fieldScale, sizeof(fieldScale));
	stream.read((char*)&timeScale, sizeof(timeScale));

	if (versionNumber >= 14)
		stream.read((char*)&scale, sizeof(scale));

}


ComponentTypes ParticleComponent::getType()
{
	return ComponentTypes::PARTICLE;
}


AiComponent* ParticleComponent::clone()
{
	return new ParticleComponent(*this);
}


void ParticleComponent::initialize(GameObject* _parent)
{

	AiComponent::initialize(_parent);


	seed.x = rand();
	seed.y = rand();
	seed.z = rand();

	ParticleEmitter::initialize();

	for (int i = 0; i < spawnCount; i++)
	{
		addParticle();
		particles[i].lifeTime -= spawnIntervall * i;
	}

}


void ParticleComponent::shutdown()
{
	ParticleEmitter::shutdown();
}


void ParticleComponent::update()
{

	spawnTimer += gameClock->getElapsedTime();

	while (spawnTimer > spawnIntervall)
	{
		addParticle();
		spawnTimer -= spawnIntervall;
	}


	for (int i = 0; i < count; i++)
	{
		sf::Vector3f v;

		sf::Vector3f p = particles[i].position;
		p.x *= fieldScale.x;
		p.y *= fieldScale.y;
		p.z *= fieldScale.z;
			
		v = getVel(p, gameClock->getTotalElapsedTime() * timeScale, seed);

		v *= 2.0f;

		MATH::clamp(v.x, -1.0f, 1.0f);
		MATH::clamp(v.y, -1.0f, 1.0f);
		MATH::clamp(v.z, -1.0f, 1.0f);

		v.x = (v.x + 1.0f) * (maxVelocity.x - minVelocity.x) / 2.0f + minVelocity.x;
		v.y = (v.y + 1.0f) * (maxVelocity.y - minVelocity.y) / 2.0f + minVelocity.y;
		v.z = (v.z + 1.0f) * (maxVelocity.z - minVelocity.z) / 2.0f + minVelocity.z;

		particles[i].velocity = v;
		particles[i].position += particles[i].velocity * gameClock->getElapsedTime();

		particles[i].lifeTime -= gameClock->getElapsedTime();

		if (particles[i].lifeTime < 0.0f)
		{
			particles[i] = particles[count - 1];

			particleManager->remove(&particles[count - 1]);

			count--;
			i--;
		}
		
	}

}

	//sf::Sprite sprite = *particles[id]->sprite;

void ParticleComponent::addParticle()
{
	if (count >= maxParticles)
		return;

	
	float z = depth * getGame()->getCamera()->getView().getSize().x / 2.0f * 1.7f / mToPx;
	
	particles[count].position.x = parent->getPosition().x + fmod(rand(), 2.0f * spawnRadius.x) - spawnRadius.x;
	particles[count].position.y = parent->getPosition().y + fmod(rand(), 2.0f * spawnRadius.y) - spawnRadius.y;
	particles[count].position.z = z + fmod(rand(), 2.0f * spawnRadius.z) - spawnRadius.z;

	particles[count].lifeTime = fmod(rand(), (maxLifeTime - minLifeTime)) + minLifeTime;
	particles[count].startTime = particles[count].lifeTime;

	particles[count].emitter = this;

	particleManager->add(&particles[count]);

	count ++;
}




sf::Vector3f ParticleComponent::getVel(sf::Vector3f& pos, float t, sf::Vector3i& seed)
{
	sf::Vector3f v;

	v.x = MATH::noise(pos, t, seed.x);
	v.y = MATH::noise(pos, t, seed.y);
	v.z = MATH::noise(pos, t, seed.z);

	return v;
}


void ParticleComponent::setDepth(float _depth)
{
	depth = _depth;
}

float ParticleComponent::getDepth()
{
	return depth;
}

void ParticleComponent::setMaxParticles(float _maxParticles)
{
	maxParticles = _maxParticles;
}

float ParticleComponent::getMaxParticles() const
{
	return maxParticles;
}

void ParticleComponent::setSpawnIntervall(float _spawnIntervall)
{
	spawnIntervall = _spawnIntervall;
}

float ParticleComponent::getSpawnIntervall() const
{
	return spawnIntervall;
}

void ParticleComponent::setSpawnRadius(sf::Vector3f radius)
{
	spawnRadius = radius;
}

sf::Vector3f ParticleComponent::getSpawnRadius()
{
	return spawnRadius;
}

void ParticleComponent::setSpawnCount(float _spawnCount)
{
	spawnCount = _spawnCount;
}

float ParticleComponent::getSpawnCount()
{
	return spawnCount;
}



void ParticleComponent::setLifeTime(sf::Vector2f lifeTime)
{
	minLifeTime = lifeTime.x;
	maxLifeTime = lifeTime.y;
}

sf::Vector2f ParticleComponent::getLifeTime()
{
	return sf::Vector2f(minLifeTime, maxLifeTime);
}

void ParticleComponent::setMinVelocity(sf::Vector3f _minVelocity)
{
	minVelocity = _minVelocity;
}

sf::Vector3f ParticleComponent::getMinVelocity()
{
	return minVelocity;
}

void ParticleComponent::setMaxVelocity(sf::Vector3f _maxVelocity)
{
	maxVelocity = _maxVelocity;
}

sf::Vector3f ParticleComponent::getMaxVelocity()
{
	return maxVelocity;
}

void ParticleComponent::setFieldScale(sf::Vector3f _fieldScale)
{
	fieldScale = _fieldScale;
}

sf::Vector3f ParticleComponent::getFieldScale()
{
	return fieldScale;
}

void ParticleComponent::setTimeScale(float _timeScale)
{
	timeScale = _timeScale;
}

float ParticleComponent::getTimeScale()
{
	return timeScale;
}