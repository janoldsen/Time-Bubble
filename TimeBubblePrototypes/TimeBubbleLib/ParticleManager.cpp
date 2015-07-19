#include "ParticleManager.h"
#include "Camera.h"
#include "Defines.h"
#include <SFML\Graphics.hpp>
#include "ParticleEmitter.h"

ParticleManager ParticleManager::instance;

ParticleManager* ParticleManager::getInstance()
{
	return &instance;
}


void ParticleManager::add(Particle* particle)
{
	particles.push_back(particle);
}


void ParticleManager::remove(Particle* particle)
{
	std::vector<Particle*>::iterator it;

	for (it = particles.begin(); it != particles.end(); it++)
	{
		if (*it == particle)
		{
			particles.erase(it);
			return;
		}
	}
}


static bool sortParticle(Particle* i, Particle* j)
{
	return i->position.z > j->position.z;
}

void ParticleManager::sort()
{
	std::sort(particles.begin(), particles.end(), sortParticle);
}


float ParticleManager::getDepth(Camera* camera, int id)
{
	if (id >= particles.size())
		return 0.0f;

	float cpZ = (camera->getView().getSize().x / 2.0f * 1.7 /mToPx);

	return (particles[id]->position.z + cpZ) / (camera->getView().getSize().x / 2.0f * 1.7 /mToPx);
}


void ParticleManager::draw(Camera* camera, sf::RenderTarget* target, int id, sf::Color color)
{

	if (id >= particles.size())
		return;


	particles[id]->emitter->draw(camera, target, particles[id], color);

	//sf::Sprite sprite = *particles[id]->sprite;

	//sf::Vector2f cP = camera->getPosition();
	//sf::Vector2f position = sf::Vector2f(particles[id]->position.x, particles[id]->position.y);
	//float depth = getDepth(camera, id);

	//if (depth < 0.0f)
	//	return;

	//sprite.setPosition((cP.x -  (cP.x - position.x * mToPx)/depth), (cP.y - (cP.y - position.y * -mToPx)/depth));
	//sprite.setScale(sf::Vector2f(1/depth, 1/depth));	

	//color.a = (-pow((2 * (particles[id]->lifeTime / particles[id]->startTime) - 1.0f),2.0f) + 1.0f)  * 255;

	//sprite.setColor(color);


	//target->draw(sprite);
}


float ParticleManager::getSize()
{
	return particles.size();
}