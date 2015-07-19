#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <vector>
#include <SFML\Graphics.hpp>

struct Particle;
class Camera;

class ParticleManager
{
public:

	static ParticleManager* getInstance();

	void add(Particle* particle);
	void remove(Particle* particle);

	void sort();
	float getDepth(Camera* camera, int id);
	void draw(Camera* camera, sf::RenderTarget* target, int id, sf::Color color = sf::Color::White);

	float getSize();
	


private:
	
	std::vector<Particle*> particles;

	static ParticleManager instance;

	ParticleManager() {}
	ParticleManager(ParticleManager& particleManager);
	ParticleManager& operator=(const ParticleManager& particleManager);

};


#define particleManager ParticleManager::getInstance()

#endif