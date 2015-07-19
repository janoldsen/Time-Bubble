#ifndef CAMERA_H
#define CAMERA_H

#include <SFML\Graphics.hpp>
#include "GameObject.h"
#include "BackGround.h"
#include "ObjectIterator.h"
#include "Level.h"
#include "PlayerComponent.h"
#include "BgObjectContainer.h"
#include "TimeBubbleComponent.h"

class Camera
{
public:

	Camera();
	
	void initialize(sf::RenderTarget* target);

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	void setPlayer(GameObject* _player, PlayerComponent* _playerAI);

	void draw(sf::RenderTarget* _renderTarget, GOIterator* gameObjects, BgObjectContainer* bgObjects, Level& level, bool debug = false);
	void draw(GOIterator* gameObjects, BgObjectContainer* bgObjects, Level& level, bool debug = false);

	void drawPolygon(b2PolygonShape* polygon, sf::Vector2f position, float angle = 0.0f);
	void drawChain(b2ChainShape* chain, sf::Vector2f position, float angle = 0.0f);
	void drawCircle(b2CircleShape* circle, sf::Vector2f position, float angle = 0.0f);



	void update(Level& level);
	
	void updateRenderTarget();

	void addTimeBubble(TimeBubbleComponent* bubble);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f _position);

	void setView(sf::View _view);
	const sf::View& getView() const;



	sf::Shader* getRenderShader();

	void setDepthEnabled(bool flag);
	bool isDepthEnabled();


	void setFogColor(sf::Vector3f color);
	sf::Vector3f getFogColor() const;
	void setFogStrength(float strength);
	float getFogStrength() const;
	void setClouds(bool flag);
	bool getClouds() const;

	float getSunStrength() const;
	void setSunStrength(float _sunStrength);

	void setZoom(float _zoom);
	float getZoom() const;

	void zoomTo(float endZoom,float time);

private:
	sf::RenderTarget* renderTarget;
	sf::RenderTexture FBO1;
	sf::RenderTexture FBO2;

	sf::View view;

	GameObject* playerObj;
	PlayerComponent* playerAi;

	sf::Vector2f position;


	void drawSunMask(GOIterator* gameObjects, BgObjectContainer* bgObjects, Level& level);
	BgOIterator* sunIt;
	sf::Shader sunShader;
	float sunStrength;


	bool depthEnabled;
	sf::Shader depthShader;
	float fogStrength;
	sf::Vector3f fogColor;
	bool clouds;

	sf::Shader bubbleShader;
	std::vector<TimeBubbleComponent*> bubbles;

	float zoom;

	float timer;
	float time;
	float endZoom;
	float startZoom;

};

#endif