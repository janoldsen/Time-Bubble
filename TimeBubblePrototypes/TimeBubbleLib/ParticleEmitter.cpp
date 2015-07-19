#include "ParticleEmitter.h"
#include "Camera.h"
#include "Defines.h"
#include "ImageManager.h"
#include "ParticleManager.h"
#include "GameClock.h"

ParticleEmitter::ParticleEmitter(void)
{
	maxParticles = 500.0f;
	scale = 1.0f;
	count = 0.0f;
	particles = 0;
}


void ParticleEmitter::initialize()
{
	particles = new Particle[maxParticles];

}

void ParticleEmitter::shutdown()
{
	for (int i = 0; i < count; i++)
		particleManager->remove(&particles[i]);
	delete [] particles;	
}

void ParticleEmitter::update()
{
	for (int i = 0; i < count; i++)
	{
		particles[i].velocity += sf::Vector3f(0.0f, -GRAVITY, 0.0f) * gameClock->getElapsedTime() * 0.1f;
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


void ParticleEmitter::draw(Camera* camera, sf::RenderTarget* target, Particle* particle, sf::Color color)
{
	sf::Vector2f cP = camera->getPosition();
	sf::Vector2f position = sf::Vector2f(particle->position.x, particle->position.y);
 	
	
	float cpZ = (camera->getView().getSize().x / 2.0f * 1.7 /mToPx);
	float depth = (particle->position.z + cpZ) / (camera->getView().getSize().x / 2.0f * 1.7 /mToPx);


	if (depth < 0.0f)
		return;

	sprite.setPosition((cP.x -  (cP.x - position.x * mToPx)/depth), (cP.y - (cP.y - position.y * -mToPx)/depth));
	sprite.setScale(sf::Vector2f(1/depth * scale, 1/depth * scale));	

	color.a = (-pow((2 * (particle->lifeTime / particle->startTime) - 1.0f),2.0f) + 1.0f)  * 255;

	sprite.setColor(color);


	target->draw(sprite);
}


void ParticleEmitter::addParticle(Particle& particle)
{
	if (count >= maxParticles)
		return;

	particles[count] = particle;
	particles[count].emitter = this;
	particleManager->add(&particles[count]);
	count++;

}


void ParticleEmitter::setSprite(std::string fileName)
{
	
	sf::Texture* tex = &imageManager->getTexture(fileName);
	sprite.setTexture(*tex, true);
	sprite.setOrigin(tex->getSize().x / 2.0f, tex->getSize().y / 2.0f);

}

std::string ParticleEmitter::getSprite()
{
	return imageManager->getFileName(sprite.getTexture());
}


void ParticleEmitter::setScale(float _scale)
{
	scale = _scale;
}

float ParticleEmitter::getScale()
{
	return scale;
}