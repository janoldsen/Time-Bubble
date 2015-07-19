#include "EditorLevel.h"
#include "ImageManager.h"
#include "Prototype.h"
#include "myMath.h"
#include "SelectionVertex.h"
#include <Box2D\Box2D.h>
#include <iostream>
#include "Selector.h"

EditorLevel::EditorLevel()
{
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
}

void EditorLevel::shutdown()
{

	setBackGround(imageManager->getTexture(""));
	setGameGround(imageManager->getTexture(""));

	selection.shutdown();

	for (int i = 0; i < shapes.size(); i++)
	{
		//delete[] shapes[i]->m_vertices;
		delete shapes[i];
	}

	for (int i = 0; i < shapes.size(); i++)
	{
		delete fixtureDefs[i]->userData;
		delete fixtureDefs[i];
	}



	fixtureDefs.clear();
	shapes.clear();
	
	for (int i = 0; i <selections.size(); i++)
	{
		for (int j = 0; j < selections[i].size(); j++)
		{
			delete selections[i][j];
		}

		selections[i].clear();
	}
	selections.clear();

	

}




void EditorLevel::drawGameGround(Camera* camera, sf::RenderTarget* target, bool debug, sf::Color color)
{
	gameGround.draw(sf::Vector2f(size.left + size.width/2.0f, size.top - size.height/2.0f ), 0.0f, camera, target, color);

	if (debug)
	{
		for (int i = 0; i < shapes.size(); i++)
		{
			camera->drawChain(shapes[i], sf::Vector2f(0.0f, 0.0f));
		}


		sf::Vector2f vCenter = target->getView().getCenter();
		sf::Vector2f vSize = target->getView().getSize();
		vSize *= 0.5f;
		
		sf::FloatRect rect;
		
		sf::Vector2f verts[8];
		verts[0] = (sf::Vector2f(vCenter.x - vSize.x, vCenter.y - vSize.y));
		verts[1] = (sf::Vector2f(vCenter.x + vSize.x, vCenter.y - vSize.y));
		verts[2] = (sf::Vector2f(vCenter.x + vSize.x, vCenter.y + vSize.y));
		verts[3] = (sf::Vector2f(vCenter.x - vSize.x, vCenter.y + vSize.y));
		


		verts[4].x = (size.left)				* mToPx;
		verts[4].y = -(size.top)				* mToPx;
		verts[5].x = (size.left + size.width)	* mToPx;
		verts[5].y = -(size.top)				* mToPx;
		verts[6].x = (size.left + size.width)	* mToPx;
		verts[6].y = -(size.top - size.height)	* mToPx;
		verts[7].x = (size.left)				* mToPx;
		verts[7].y = -(size.top - size.height)	* mToPx;


		sf::ConvexShape levelSize(4);
		levelSize.setFillColor(sf::Color(50,50,50,150));
		levelSize.setPoint(0, verts[0]);
		levelSize.setPoint(1, verts[4]);
		levelSize.setPoint(2, verts[7]);
		levelSize.setPoint(3, verts[3]);

		target->draw(levelSize);

		levelSize.setPoint(0, verts[0]);
		levelSize.setPoint(1, verts[1]);
		levelSize.setPoint(2, verts[5]);
		levelSize.setPoint(3, verts[4]);

		target->draw(levelSize);

		levelSize.setPoint(0, verts[1]);
		levelSize.setPoint(1, verts[2]);
		levelSize.setPoint(2, verts[6]);
		levelSize.setPoint(3, verts[5]);

		target->draw(levelSize);

		levelSize.setPoint(0, verts[6]);
		levelSize.setPoint(1, verts[2]);
		levelSize.setPoint(2, verts[3]);
		levelSize.setPoint(3, verts[7]);

		target->draw(levelSize);


	}
}





void EditorLevel::writeBody(std::fstream& stream)
{
	unsigned int numberOfFixtures = fixtureDefs.size();

	stream.write((char*)&numberOfFixtures, sizeof(numberOfFixtures));

	for (int i = 0; i < numberOfFixtures; i++)
	{

		stream.write((char*)&shapes[i]->m_count, sizeof(shapes[i]->m_count));
		stream.write((char*)shapes[i]->m_vertices, shapes[i]->m_count * sizeof(b2Vec2));

		stream.write((char*)&fixtureDefs[i]->friction, sizeof(fixtureDefs[i]->friction));
		stream.write((char*)&fixtureDefs[i]->restitution, sizeof(fixtureDefs[i]->restitution));
		stream.write((char*)(int*)(fixtureDefs[i]->userData), sizeof(int));

	}


}


void EditorLevel::readBody(std::fstream& stream, Prototype* game)
{
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	
	unsigned int numberOfFixtures;
	stream.read((char*)&numberOfFixtures, sizeof(numberOfFixtures));

	for (int i = 0; i < numberOfFixtures; i++)
	{
		b2FixtureDef fixtureDef;


		int vertexCount;
		b2Vec2* vertices;

		stream.read((char*)&vertexCount, sizeof(vertexCount));
		vertices = new b2Vec2[vertexCount];
		stream.read((char*)vertices, vertexCount * sizeof(b2Vec2));

		


		stream.read((char*)&fixtureDef.friction, sizeof(fixtureDef.friction));
		stream.read((char*)&fixtureDef.restitution, sizeof(fixtureDef.restitution));
		int userData;
		stream.read((char*)&userData, sizeof(int));
		fixtureDef.userData = new int(userData);


		//fixtureDefs.push_back(fixtureDef);

		addSegment(fixtureDef);
		setVertices(*selector, i, vertices, vertexCount);

		delete[] vertices;

	}


}



Level EditorLevel::getLevel(Prototype* game)
{
	Level level;

	level.setSize(size);

	//level.setBackGround(imageManager->getTexture(imageManager->getFileName(backGround.getTexture())));	
	//level.setGameGround(imageManager->getTexture(imageManager->getFileName(gameGround.getTexture())));

	level.setBackGround(backGround);
	level.setGameGround(gameGround);


	level.setBody(&bodyDef, game->world);



	for (int i = 0;i < fixtureDefs.size(); i++)
	{
		int* userData = (int*)fixtureDefs[i]->userData;
		fixtureDefs[i]->userData = new int(*userData);

		level.addFixture(fixtureDefs[i]);

		fixtureDefs[i]->userData = userData;
	}

	

	return level;
}




void EditorLevel::checkSelection(sf::FloatRect& selectionRect)
{

	for (int i = 0; i < shapes.size(); i++)
	{
		for (int j = 0; j < shapes[i]->m_count; j++)
		{
			sf::Vector2f vertex = MATH::b2ToSf(shapes[i]->m_vertices[j]);

			if (selectionRect.contains(vertex))
			{
				

				selections[i][j]->select(true);

			}
		}

	}

	
}




int EditorLevel::addSegment(b2FixtureDef& fixtureDef)
{
	fixtureDefs.push_back(new b2FixtureDef(fixtureDef));

	b2ChainShape* shape = new b2ChainShape();

	shapes.push_back(shape);

	fixtureDefs.back()->shape = shapes.back();

	selections.push_back(std::vector<SelectionVertex*>());

	selection.addSegment();

	return fixtureDefs.size()-1;

}

void EditorLevel::setVertices(Selector& selector, int segmentId, b2Vec2* vertices, int count)
{
	shapes[segmentId]->CreateChain(vertices, count);

	for (int i = 0; i < count; i++)
	{
		selections[segmentId].push_back(new SelectionVertex(selector, this, &shapes[segmentId]->m_vertices[i], segmentId, i));

		selection.addVertex(segmentId, selections[segmentId].back()->getPropertyBox());
		selections[segmentId].back()->updateProperty();

	}

}

void EditorLevel::addVertex(Selector& selector, int segmentId, b2Vec2 position)
{

	b2ChainShape** shape = &shapes[segmentId];

	if ((*shape)->m_count < 2)
	{
		std::cout << "CANNOT ADD VERTEX - chain too short" << std::endl;
		return;
	}

	b2Vec2* newVertices = new b2Vec2[(*shape)->m_count+1];
	
	for (int i = 0; i < (*shape)->m_count; i++)
	{
		newVertices[i] = (*shape)->m_vertices[i];		
	}

	int count = (*shape)->m_count + 1;
	newVertices[count-1] = position;

	delete *shape;
	*shape = new b2ChainShape();
	fixtureDefs[segmentId]->shape = *shape;

	(*shape)->CreateChain(newVertices,count);


	for (int i = 0; i < (*shape)->m_count-1; i++)
	{
		selections[segmentId][i]->setVertex(&(*shape)->m_vertices[i], segmentId, i);
	}
	selections[segmentId].push_back(new SelectionVertex(selector,this, &(*shape)->m_vertices[count - 1], segmentId, count - 1));



	selection.addVertex(segmentId, selections[segmentId].back()->getPropertyBox());
	selections[segmentId].back()->updateProperty();

	delete [] newVertices;

}


void EditorLevel::insertVertex(Selector& selector, int vertexId1, int segmentId)
{

	b2ChainShape** shape = &shapes[segmentId];
	int count = (*shape)->m_count + 1;

	b2Vec2* newVertices = new b2Vec2[count];
	
	for (int i = 0; i < vertexId1 + 1; i++)
	{
		newVertices[i] = (*shape)->m_vertices[i];		
	}

	b2Vec2 newVertex = ((*shape)->m_vertices[vertexId1] + (*shape)->m_vertices[vertexId1 + 1]);
	newVertex *= 0.5;

	newVertices[vertexId1 + 1] = newVertex;

	for (int i = vertexId1 + 1; i < (*shape)->m_count; i++)
	{
		newVertices[i+1] = (*shape)->m_vertices[i];
	}


	delete *shape;
	*shape = new b2ChainShape();
	fixtureDefs[segmentId]->shape = *shape;

	(*shape)->CreateChain(newVertices,count);

	for (int i = 0; i < /*(*shape)->m_count-1*/ vertexId1 + 1; i++)
	{
		selections[segmentId][i]->setVertex(&(*shape)->m_vertices[i], segmentId, i);
	}
	selections[segmentId].insert(selections[segmentId].begin() + vertexId1 + 1, new SelectionVertex(selector,this, &(*shape)->m_vertices[vertexId1 + 1], segmentId, vertexId1 + 1));
	selection.insertVertex(segmentId, vertexId1 + 1, selections[segmentId][vertexId1 +1]->getPropertyBox());

	for (int i = vertexId1 + 2; i < count; i++)
	{
		selections[segmentId][i]->setVertex(&(*shape)->m_vertices[i], segmentId, i);
	}

	selections[segmentId].back()->updateProperty();
	
	delete[] newVertices;
}

void EditorLevel::breakVertex(Selector& selector, int vertexId, int segmentId)
{

	b2ChainShape** shape = &shapes[segmentId];


	int size1 = vertexId+1;
	int size2 = (*shape)->m_count - vertexId;

	if (size1 < 2 || size2 <2)
	{
		std::cout << "cannot break first or last Vertex!" << std::endl;
		return;
	}


	b2Vec2* newVertices1 = new b2Vec2[size1];
	b2Vec2* newVertices2 = new b2Vec2[size2];

	for (int i = 0; i < size1; i++)
	{
		newVertices1[i] = (*shape)->m_vertices[i];		
	}

	for (int i = (size1-1); i < (*shape)->m_count; i++)
	{
		newVertices2[i - (size1 -1)] = (*shape)->m_vertices[i];
	}


	newVertices1[size1-1].y += 0.5f;
	newVertices2[0].y -= 0.5f;


	for (int i = size1; i < (*shape)->m_count; i++)
	{
		selection.deleteVertex(segmentId, selections[segmentId][i]->getPropertyBox());
		delete selections[segmentId][i];
	}
	selections[segmentId].erase(selections[segmentId].begin() + size1, selections[segmentId].end());

	delete *shape;
	*shape = new b2ChainShape();
	fixtureDefs[segmentId]->shape = *shape;

	(*shape)->CreateChain(newVertices1, size1);

	for (int i = 0; i < size1; i++)
	{
		selections[segmentId][i]->setVertex(&(*shape)->m_vertices[i], segmentId, i);
	}


	b2FixtureDef fixtureDef = *fixtureDefs[segmentId];
	fixtureDef.userData = new int(*((int*)fixtureDef.userData)); 

	int id = addSegment(fixtureDef);

	setVertices(selector, id, newVertices2, size2);


	delete[] newVertices1;
	delete[] newVertices2;

}



void EditorLevel::weldVertices(Selector& selector, int vertexId1, int segmentId1, int vertexId2, int segmentId2)
{
	b2ChainShape** shape1 = &shapes[segmentId1];
	b2ChainShape** shape2 = &shapes[segmentId2];

	int count1 = (*shape1)->m_count;
	int count2 = (*shape2)->m_count;

	if (vertexId1 != 0 && vertexId1 != (count1-1) ||
		vertexId2 != 0 && vertexId2 != (count2-1))
	{
		std::cout << "only endvertices can be welded" << std::endl;
		return;
	}


	int count = count1 + count2;
	b2Vec2* newVertices = new b2Vec2[count];

	if (vertexId1 != 0)
	{
		for (int i = 0; i < count1; i++)
		{
			newVertices[i] = (*shape1)->m_vertices[i];
		}
	}
	else
	{
		for (int i = 0; i < count1; i++)
		{
			newVertices[i] = (*shape1)->m_vertices[count1 - i - 1];
		}
	}

	if (vertexId2 == 0)
	{
		for (int i = 0; i < count2; i++)
		{
			newVertices[i + count1] = (*shape2)->m_vertices[i];
		}
	}
	else
	{
		for (int i = 0; i < count2; i++)
		{
			newVertices[i + count1] = (*shape2)->m_vertices[count2- i - 1];
		}
	}

	delete *shape1;
	*shape1 = new b2ChainShape();
	fixtureDefs[segmentId1]->shape = *shape1;
	
	(*shape1)->CreateChain(newVertices,count);


	for (int i = 0; i < count1; i++)
	{
		selections[segmentId1][i]->setVertex(&(*shape1)->m_vertices[i], segmentId1, i);
	}

	for (int i = count1; i < count; i++)
	{
		selections[segmentId1].push_back(new SelectionVertex(selector, this, &shapes[segmentId1]->m_vertices[i], segmentId1, i));

		selection.addVertex(segmentId1, selections[segmentId1].back()->getPropertyBox());
		selections[segmentId1].back()->updateProperty();
	}


	deleteSegment(segmentId2);

	delete[] newVertices;

}


void EditorLevel::setLoop(int segmentId, bool loop)
{

	int count = shapes[segmentId]->m_count;
	b2Vec2* newVertices = new b2Vec2[count];

	for (int i = 0; i < count; i++)
	{
		newVertices[i] = shapes[segmentId]->m_vertices[i];
	}


	delete shapes[segmentId];

	shapes[segmentId] = new b2ChainShape();

	if (loop)
		shapes[segmentId]->CreateLoop(newVertices, count);
	else
		shapes[segmentId]->CreateChain(newVertices, count-1);

	selector->clearSelection();

	delete [] newVertices;
}


void EditorLevel::deleteSegment(int segmentId)
{

	selection.deleteSegment(segmentId);

	for (int i = segmentId+1; i < selections.size(); i++)
	{
		for (int j = 0; j < selections[i].size(); j++)
		{
			selections[i][j]->setSegmentId(i - 1);
		}


	}


	for (int i = 0; i < selections[segmentId].size(); i++)
	{
		selections[segmentId][i]->unSelect();

		delete selections[segmentId][i];
	}

	selections.erase(selections.begin() + segmentId);


	delete shapes[segmentId];
	shapes.erase(shapes.begin() + segmentId);


	delete fixtureDefs[segmentId]->userData;
	delete fixtureDefs[segmentId];
	fixtureDefs.erase(fixtureDefs.begin() + segmentId);



}

void EditorLevel::deleteVertex(int segmentId, int vertexId)
{
	
	b2ChainShape** shape = &shapes[segmentId];

	if ((*shape)->m_count <= 2)
	{
		deleteSegment(segmentId);
		return;
	}

	b2Vec2* newVertices = new b2Vec2[(*shape)->m_count-1];
	
	int j = 0;
	for (int i = 0; i < (*shape)->m_count; i++)
	{
		if (i != vertexId)
		{
			newVertices[j] = (*shape)->m_vertices[i];
			j++;
		}
	}

	int count = (*shape)->m_count - 1;

	delete *shape;
	*shape = new b2ChainShape();
	fixtureDefs[segmentId]->shape = *shape;

	(*shape)->CreateChain(newVertices,count);

	
	selection.deleteVertex(segmentId, selections[segmentId][vertexId]->getPropertyBox());
	
	delete selections[segmentId][vertexId];
	selections[segmentId].erase(selections[segmentId].begin() + vertexId);


	for (int i = 0; i < count; i++)
	{
		selections[segmentId][i]->setVertex(&(*shape)->m_vertices[i], segmentId, i);
	}

	selections[segmentId].back()->updateProperty();

	delete[] newVertices;
}



void EditorLevel::setSelection(Selector* _selector, sfg::Box::Ptr* hierarchy)
{

	selection.setLevel(*this);
	selection.set(_selector, hierarchy);

	selector = _selector;

}