#include "Camera.h"
#include <iostream>
#include "Defines.h"
#include "myMath.h"
#include "GameClock.h"
#include "ImageManager.h"
#include "ParticleManager.h"


Camera::Camera()
{
	
	zoom = 2.0f;

	timer = -1.0f;
	
	fogColor = sf::Vector3f(0.0f,0.0f,0.0f);
	fogStrength = 0.02f;
	clouds = false;

	sunIt = 0;

	sunStrength = 3.0f;

}


void Camera::initialize(sf::RenderTarget* target)
{
	renderTarget = target;
	FBO1.create(renderTarget->getSize().x, renderTarget->getSize().y);
	FBO2.create(renderTarget->getSize().x, renderTarget->getSize().y);


	playerObj = NULL;
	playerAi = NULL;

	position.x = 0.0f;
	position.y = 0.0f;

	//sf::View view = renderTarget->getView();
	view.setCenter(position);
	view.setSize(800 * zoom, 600 * zoom);

	//renderTarget->setView(view);



	//clouds = false;


	depthShader.loadFromFile("shader/distanceFog.frag", sf::Shader::Fragment);
	depthShader.setParameter("fog_Strength", fogStrength);
	depthShader.setParameter("fog_Color", fogColor.x, fogColor.y, fogColor.z, 1.0);
	depthShader.setParameter("clouds", clouds);

	fogStrength = 0.02;

	depthEnabled = true;

	sunIt = 0;
	sunShader.loadFromFile("shader/light_ray.frag", sf::Shader::Fragment);

	bubbleShader.loadFromFile("shader/bubble.frag", sf::Shader::Fragment);

	timer = -1.0f;

}


void Camera::write(std::fstream& stream)
{
	stream.write((char*)&fogColor, sizeof(fogColor));
	stream.write((char*)&fogStrength, sizeof(fogStrength));

	stream.write((char*)&fogColor, sizeof(fogColor));
	stream.write((char*)&zoom, sizeof(zoom));

	stream.write((char*)&sunStrength, sizeof(sunStrength));

}


void Camera::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&fogColor, sizeof(fogColor));
	setFogColor(fogColor);
	stream.read((char*)&fogStrength, sizeof(fogStrength));
	setFogStrength(fogStrength);

	if (versionNumber >= 6)
	{
		stream.read((char*)&fogColor, sizeof(fogColor));
		setFogColor(fogColor);

		stream.read((char*)&zoom ,sizeof(zoom));

	}

	if (versionNumber >= 11)
		stream.read((char*)&sunStrength, sizeof(sunStrength));
}





void Camera::setPlayer(GameObject* _playerObj, PlayerComponent* _playerAi)
{
	playerObj = _playerObj;
	playerAi = _playerAi;
}


void Camera::update(Level& level)
{

	if (!playerObj)
		return;

	sf::Vector2f playerObjPos = playerObj->getPosition();
	playerObjPos.x *= mToPx;
	playerObjPos.y *= -mToPx;


	bool posChanged = false;
	
	if (playerObjPos.x > position.x + 50)
		position.x = playerObjPos.x - 50;

	if (playerObjPos.x < position.x - 50)
		position.x = playerObjPos.x + 50;

	if (playerObjPos.y > position.y + 300)
		position.y = playerObjPos.y - 300;

	if (playerObjPos.y < position.y - -50)
		position.y = playerObjPos.y + -50;

	
	//sf::View view = renderTarget->getView();

	sf::Vector2f size = view.getSize();
	size /= 2.0f * mToPx;
	sf::Vector2f pos;
	pos.x = position.x / mToPx;
	pos.y = -position.y / mToPx;

	if (pos.x - size.x < level.getSize().left)
		pos.x = level.getSize().left + size.x;
	if (pos.x + size.x > level.getSize().left + level.getSize().width)
		pos.x = level.getSize().left + level.getSize().width - size.x;
	if (pos.y + size.y > level.getSize().top)
		pos.y = level.getSize().top - size.y;
	if (pos.y - size.y < level.getSize().top - level.getSize().height)
		pos.y = level.getSize().top - level.getSize().height + size.y;

	position.x = pos.x * mToPx;
	position.y = -pos.y * mToPx;

	view.setCenter(position);

	if (timer > 0.0f)
	{
		timer -= gameClock->getElapsedTime();
		
		float _zoom = (timer/time * startZoom + (1.0 - timer/time) * endZoom);
		view.setSize(800 * _zoom, 600 * _zoom);

	}


	//renderTarget->setView(view);








}



void Camera::drawSunMask(GOIterator* gameObjects, BgObjectContainer* bgObjects, Level& level)
{
	


	sunIt->object->draw(this, &FBO1);

	float particleId = 0.0f;

	BgOIterator* front = 0;
	for (BgOIterator* it = sunIt->next; it; it = it->next)
	{
		while(particleManager->getDepth(this, particleId) > it->object->getDepth() && particleId < particleManager->getSize())
		{
			particleManager->draw(this, &FBO1, particleId);
			particleId++;
		}

		if (it->object->getDepth() < 1.0f)
		{
			front = it;
			break;
		}
		it->object->draw(this, &FBO1, sf::Color::Black);



	}
	
	level.drawGameGround(this, &FBO1, false, sf::Color::Black);


	while(particleManager->getDepth(this, particleId) > 1.0f && particleId < particleManager->getSize())
	{
		particleManager->draw(this, &FBO1, particleId);
		particleId++;
	}


	//for (GOIterator* it = gameObjects; it; it = it->next)
	//{
	//	it->object->draw(this, &FBO1, false);
	//}

	if (front)
	{
		for (BgOIterator* it = front; it; it = it->next)
		{
			it->object->draw(this, &FBO1, sf::Color::Black);
		}

	}

}

void Camera::draw(GOIterator* gameObjects, BgObjectContainer* bgObjects, Level& level, bool debug)
{

	FBO1.setView(view);
	//FBO2.setView(view);

	depthShader.setParameter("time", gameClock->getTotalElapsedTime());

	particleManager->sort();


	if (sunIt)
	{
		FBO1.clear(sf::Color::Transparent);
		drawSunMask(gameObjects, bgObjects, level);
		
		sf::Vector2f sunPos = (sf::Vector2f)FBO1.mapCoordsToPixel(sunIt->object->getSprite().getPosition());

		sunPos.x /= FBO1.getSize().x;
		sunPos.y /= FBO1.getSize().y;

		sunPos.y = 1.0f - sunPos.y;

		sunShader.setParameter("sun_pos", sunPos);
		sunShader.setParameter("weight", sunStrength);

		FBO1.display();

		FBO2.clear(sf::Color::Transparent);
		FBO2.draw(sf::Sprite(FBO1.getTexture()), &sunShader);
		FBO2.display();

	}


	FBO1.clear();

	

	int particleId = 0;

	level.drawBackGround(this, &FBO1, debug);
	
	BgOIterator* front = 0;
	sunIt = 0;
	for (BgOIterator* it = bgObjects->begin; it; it = it->next)
	{

		while(particleManager->getDepth(this, particleId) > it->object->getDepth() && particleId < particleManager->getSize())
		{
			particleManager->draw(this, &FBO1, particleId);
			particleId++;
		}

		if (it->object->getDepth() < 1.0f)
		{
			front = it;
			break;
		}
		it->object->draw(this, &FBO1);

			

		if (it->object->getName() == "sun")
			sunIt = it;

	}

	
	
	level.drawGameGround(this, &FBO1, debug);

	while(particleManager->getDepth(this, particleId) > 1.0f && particleId < particleManager->getSize())
	{
		particleManager->draw(this, &FBO1, particleId);
		particleId++;
	}


	
	for (GOIterator* it = gameObjects; it; it = it->next)
	{
		it->object->draw(this, &FBO1, debug);
	}



	if (sunIt)
	{
		FBO1.setView(FBO1.getDefaultView());
		FBO1.draw(sf::Sprite(FBO2.getTexture()), sf::BlendAdd);
		FBO1.setView(view);
	}

	
	
	FBO1.display();

	FBO2.draw(sf::Sprite(FBO1.getTexture()));

	for (int i = 0; i < bubbles.size(); i++)
	{
		sf::CircleShape circleShape;
		float r = bubbles[i]->getRadius() * mToPx;
		sf::Vector2f p = MATH::vMtoPx(bubbles[i]->getParent()->getPosition());
		circleShape.setRadius(r);
		circleShape.setOrigin(r,r);
		circleShape.setPosition(p);

		p = (sf::Vector2f)FBO2.mapCoordsToPixel(p, view);
		p.y = FBO2.getSize().y - p.y;

		r *= FBO1.getSize().x / view.getSize().x;

		bubbleShader.setParameter("texture", FBO2.getTexture());
		bubbleShader.setParameter("position", p);
		bubbleShader.setParameter("radius", r);
		bubbleShader.setParameter("size", (sf::Vector2f)FBO2.getSize());

		FBO1.draw(circleShape, &bubbleShader);
	}
	bubbles.clear();

	
	if (front)
	{



		for (BgOIterator* it = front; it; it = it->next)
		{

			while(particleManager->getDepth(this, particleId) > it->object->getDepth()  && particleId < particleManager->getSize())
			{
				particleManager->draw(this, &FBO1, particleId);
				particleId++;
			}

			it->object->draw(this, &FBO1);
		}

	}


	while(particleId < particleManager->getSize())
	{
		particleManager->draw(this, &FBO1, particleId);
		particleId++;
	}


	FBO1.display();

	renderTarget->draw(sf::Sprite(FBO1.getTexture()));
	
	//HUD

	

	if (playerAi)
	{

		if (playerAi->getCurrWeapon() != PlayerComponent::Weapon::NONE)
		{

			
			//spr.setOrigin(112.0f / 2.0f, 150.0f / 2.0f);



			float slow =  playerAi->getNumBubbles().y / 15.0f;
			sf::Sprite slowSpr(imageManager->getTexture("res/hud_blue.png"));
			sf::Vector2f size = (sf::Vector2f)slowSpr.getTexture()->getSize();
			slowSpr.setPosition(44.0f, (renderTarget->getSize().y -  150) + (1.0f - slow) * size.y + 88.0f);
			slowSpr.setTextureRect(sf::IntRect(0.0f, size.y * (1.0f - slow), size.x, size.y * slow));

			renderTarget->draw(slowSpr);

			float fast =  playerAi->getNumBubbles().x / 15.0f;
			sf::Sprite fastSpr(imageManager->getTexture("res/hud_red.png"));
			size = (sf::Vector2f)fastSpr.getTexture()->getSize();
			fastSpr.setPosition(0.0f, (renderTarget->getSize().y -  150.0f) + (1.0f - fast) * size.y + 1.0f);
			fastSpr.setTextureRect(sf::IntRect(0.0f, size.y * (1.0f - fast), size.x, size.y * fast));

			renderTarget->draw(fastSpr);


			sf::Sprite spr(imageManager->getTexture("res/hud_frame.png"));
			spr.setPosition(0, renderTarget->getSize().y - 150);
			renderTarget->draw(spr);

		}

		//renderTarget->setView(oldView);
	}
	
}


void Camera::draw(sf::RenderTarget* _renderTarget, GOIterator* gameObjects, BgObjectContainer* bgObjects,Level& level, bool debug)
{ 
	sf::RenderTarget* oldTarget = renderTarget;

	renderTarget = _renderTarget;
	updateRenderTarget();

	draw(gameObjects, bgObjects, level, debug);

	renderTarget = oldTarget;
	updateRenderTarget();


}

void Camera::drawPolygon(b2PolygonShape* polygon, sf::Vector2f position, float angle)
{
	
	sf::ConvexShape shape;
	shape.setFillColor(sf::Color(0,0,255,50));
	shape.setOutlineColor(sf::Color(0,0,255,255));
	shape.setOutlineThickness(2);
	shape.setPointCount(polygon->GetVertexCount());


	for (int i = 0; i < polygon->GetVertexCount(); i++)
	{

		

		sf::Vector2f point;
		point.x =  ( (cos(angle) * polygon->GetVertex(i).x - sin(angle) * polygon->GetVertex(i).y)  + position.x )* mToPx;
		point.y = -( (cos(angle) * polygon->GetVertex(i).y + sin(angle) * polygon->GetVertex(i).x)  + position.y )* mToPx;

		shape.setPoint(i, point);
	}

	FBO1.draw(shape);



}

void Camera::drawChain(b2ChainShape* chain, sf::Vector2f position, float angle)
{

	sf::VertexArray line(sf::PrimitiveType::LinesStrip, chain->m_count);

	for (int i = 0; i < chain->m_count; i++)
	{
		line[i].color = sf::Color(255,0,0,255);
		line[i].position.x =  ( (cos(angle) * chain->m_vertices[i].x - sin(angle) * chain->m_vertices[i].y) + position.x ) * mToPx;
		line[i].position.y = -( (cos(angle) * chain->m_vertices[i].y + sin(angle) * chain->m_vertices[i].x) + position.y) * mToPx;
	}	


	for (int i = 0; i < chain->m_count; i++)
	{
		sf::CircleShape circle;
		circle.setFillColor(line[i].color);

		circle.setRadius(4);
		circle.setOrigin(4.0f, 4.0f);
		circle.setPosition(line[i].position);


		FBO1.draw(circle);

	}


	for (int i = 0; i < 3; i++)
	{
		FBO1.draw(line);
		for (int i = 0; i < chain->m_count; i++)
		{
			line[i].position.y ++;
		}	
		
	}






}


void Camera::drawCircle(b2CircleShape* circle, sf::Vector2f position, float angle)
{

	sf::CircleShape shape;
	shape.setFillColor(sf::Color(255,255,0,50));
	shape.setOutlineColor(sf::Color(255,255,0,255));
	shape.setOutlineThickness(3);

	sf::Vector2f center;

	center.x =  ( (cos(angle) * circle->m_p.x - sin(angle) * circle->m_p.y)  + position.x )* mToPx;
	center.y = -( (cos(angle) * circle->m_p.y + sin(angle) * circle->m_p.x)   + position.y )* mToPx;
	

	shape.setPosition(center);
	shape.setRadius(circle->m_radius * mToPx);
	shape.setOrigin(shape.getRadius(), shape.getRadius());

	FBO1.draw(shape);

}



void Camera::updateRenderTarget()
{
	FBO1.create(renderTarget->getSize().x, renderTarget->getSize().y);
	FBO2.create(renderTarget->getSize().x, renderTarget->getSize().y);
}


void Camera::addTimeBubble(TimeBubbleComponent* bubble)
{
	bubbles.push_back(bubble);
}


sf::Vector2f Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(sf::Vector2f _position)
{
	

	//sf::View view = renderTarget->getView();

	view.move(_position - position);
	
	//renderTarget->setView(view);

	position = _position;
}



void Camera::setView(sf::View _view)
{
	view = _view;
}

const sf::View& Camera::getView() const
{
	//return renderTarget->getDefaultView();
	return view;
}


sf::Shader* Camera::getRenderShader()
{
	return &depthShader;
}



void Camera::setDepthEnabled(bool flag)
{
	depthEnabled = flag;
}

bool Camera::isDepthEnabled()
{
	return depthEnabled;
}



void Camera::setFogColor(sf::Vector3f color)
{
	depthShader.setParameter("fog_Color", color.x, color.y, color.z, 1.0);
	fogColor = color;
}

sf::Vector3f Camera::getFogColor() const
{
	return fogColor;
}

void Camera::setFogStrength(float strength)
{
	depthShader.setParameter("fog_Strength", strength);
	fogStrength = strength;
}


float Camera::getFogStrength() const
{
	return fogStrength;
}


void Camera::setClouds(bool _clouds)
{
	depthShader.setParameter("clouds", _clouds);
	clouds = _clouds;
}


bool Camera::getClouds() const
{
	return clouds;
}


void Camera::setSunStrength(float _sunStrength) 
{
	sunStrength = _sunStrength;
}


float Camera::getSunStrength() const
{
	return sunStrength;
}


void Camera::setZoom(float _zoom)
{
	zoom = _zoom;
}

float Camera::getZoom() const
{
	return zoom;
}

void Camera::zoomTo(float _endZoom, float _time)
{
	if (timer < 0.0f)
	{
		timer = _time;
		time = _time;
		endZoom = _endZoom;
		startZoom = view.getSize().x / 800.0f;
	}
}