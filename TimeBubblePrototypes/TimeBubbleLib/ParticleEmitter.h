#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <SFML\Graphics.hpp>



class ParticleEmitter;
class Camera;
struct Particle
{
	sf::Vector3f position;
	sf::Vector3f velocity;
	float startTime;
	float lifeTime;
	ParticleEmitter* emitter;
};



class ParticleEmitter
{
public:
	ParticleEmitter();
	
	void initialize();
	void shutdown();
	void update();
	void draw(Camera* camera, sf::RenderTarget* target, Particle* particle, sf::Color color = sf::Color::White);
	
	void addParticle(Particle& particle);

	void setSprite(std::string fileName);
	std::string getSprite();

	void setScale(float _scale);
	float getScale();



protected:
	Particle* particles;
	int maxParticles;
	int count;
	float scale;

	sf::Sprite sprite;



};

#endif