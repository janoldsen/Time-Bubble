#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include "AiComponent.h"
#include "ParticleEmitter.h"


class ParticleComponent : public AiComponent, public ParticleEmitter
{
public:
	ParticleComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();

	void initialize(GameObject* _parent);
	void shutdown();
	void update();

	void draw(Camera* camera, sf::RenderTarget* target, Particle* particle, sf::Color color = sf::Color::White);

	void addParticle();

	void setDepth(float _depth);
	float getDepth();

	void setMaxParticles(float _maxParticles);
	float getMaxParticles() const;

	void setSpawnIntervall(float _spawnIntervall);
	float getSpawnIntervall() const;

	void setSpawnRadius(sf::Vector3f radius);
	sf::Vector3f getSpawnRadius();

	void setSpawnCount(float _spawnCount);
	float getSpawnCount();



	void setLifeTime(sf::Vector2f lifeTime);
	sf::Vector2f getLifeTime();

	void setMinVelocity(sf::Vector3f _minVelocity);
	sf::Vector3f getMinVelocity();

	void setMaxVelocity(sf::Vector3f _maxVelocity);
	sf::Vector3f getMaxVelocity();

	void setFieldScale(sf::Vector3f _fieldScale);
	sf::Vector3f getFieldScale();

	void setTimeScale(float _timeScale);
	float getTimeScale();

private:

	sf::Vector3f getVel(sf::Vector3f& pos, float t, sf::Vector3i& seed);

	//Particle* particles;
	//int maxParticles;
	//int count;

	float depth;
	
	sf::Vector3i seed;

	float spawnIntervall;
	float spawnTimer;

	sf::Vector3f spawnRadius;

	float spawnCount;

	//sf::Sprite sprite;
	//float scale;
	float minLifeTime;
	float maxLifeTime;
	sf::Vector3f minVelocity;
	sf::Vector3f maxVelocity;

	sf::Vector3f fieldScale;
	float timeScale;

	

};
  
#endif