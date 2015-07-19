#include "PropertyBoxBody.h"
#include "LevelObject.h"
#include <myMath.h>


PropertyBoxBody::PropertyBoxBody()
{
}



void PropertyBoxBody::setObject(LevelObject* _object)
{
	object = _object;
}

void PropertyBoxBody::shutdown()
{
	for(int i = 0; i < colliders.size(); i++)
	{
		colliders[i]->shutdown();
		delete colliders[i];
	}

	colliders.clear();
	
}



void PropertyBoxBody::build()
{

	box = sfg::Box::Create(sfg::Box::VERTICAL);
	box->SetSpacing(3.0f);


	sfg::Box::Ptr angleBox = sfg::Box::Create();

	angleBox->Pack(sfg::Label::Create("Angle:"));

	angle = sfg::Entry::Create();
	angle->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxBody::setAngle, this);
	
	angleBox->Pack(angle);

	box->Pack(angleBox);



	sfg::Box::Ptr typeBox = sfg::Box::Create();

	typeBox->Pack(sfg::Label::Create("BodyType:"));

	type = sfg::ComboBox::Create();
	type->AppendItem("static");
	type->AppendItem("kinematic");
	type->AppendItem("dynamic");
	type->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxBody::setType, this);

	typeBox->Pack(type);
	box->Pack(typeBox);

	sfg::Box::Ptr activeBox = sfg::Box::Create();

	activeBox->Pack(sfg::Label::Create("Active:"));

	active = sfg::ComboBox::Create();
	active->AppendItem("false");
	active->AppendItem("true");
	active->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxBody::setActive, this);

	activeBox->Pack(active);
	box->Pack(activeBox);


	sfg::Box::Ptr fixedRotationBox = sfg::Box::Create();

	fixedRotationBox->Pack(sfg::Label::Create("FixedRotation:"));

	fixedRotation = sfg::ComboBox::Create();
	fixedRotation->AppendItem("false");
	fixedRotation->AppendItem("true");
	fixedRotation->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxBody::setFixedRotation, this);

	fixedRotationBox->Pack(fixedRotation);
	box->Pack(fixedRotationBox);


	sfg::Box::Ptr bulletBox = sfg::Box::Create();

	bulletBox->Pack(sfg::Label::Create("IsBullet:"));

	bullet = sfg::ComboBox::Create();
	bullet->AppendItem("false");
	bullet->AppendItem("true");
	bullet->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxBody::setBullet, this);

	bulletBox->Pack(bullet);
	box->Pack(bulletBox);


	sfg::Box::Ptr gravityScaleBox = sfg::Box::Create();

	gravityScaleBox->Pack(sfg::Label::Create("GravityScale:"));
	
	gravityScale = sfg::Entry::Create();
	gravityScale->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBody::setGravityScale, this);
	gravityScaleBox->Pack(gravityScale);
	box->Pack(gravityScaleBox);

	
	sfg::Box::Ptr addBoxCircleBox = sfg::Box::Create();
	
	addBox = sfg::Button::Create("Add Boxcollider");
	addBox->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxBody::doAddBox, this);
	addBoxCircleBox->Pack(addBox);

	addCircle = sfg::Button::Create("Add CircleCollider");
	addCircle->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxBody::doAddCircle, this);
	addBoxCircleBox->Pack(addCircle);

	box->Pack(addBoxCircleBox);


	for (int i = 0; i < object->fixtureDefs.size(); i++)
	{
		colliders.push_back(new PropertyBoxCollider());
		colliders.back()->setCollider(object, i);
		colliders.back()->setParent(this);
		colliders.back()->build();
		colliders.back()->update();

		//std::stringstream ss;

		//ss << "Collider" << i;

		//box->Pack(sfg::Separator::Create());
		//box->Pack(sfg::Label::Create(ss.str()));
		//box->Pack(colliders.back()->get());

		box->Pack(colliders.back()->get());

	}
	


}



void PropertyBoxBody::update()
{

	b2BodyDef& bd = object->bodyDef;

	if (!angle->HasFocus())
	{
		std::stringstream ss;
		ss << bd.angle * 180.0f/ b2_pi;
		angle->SetText(ss.str());
	}


	if (!type->HasFocus())
	{
		type->SelectItem((int) bd.type);
	}

	if (!active->HasFocus())
	{
		active->SelectItem((int) bd.active);
	}

	if (!fixedRotation->HasFocus())
	{
		fixedRotation->SelectItem((int) bd.fixedRotation);
	}

	if (!bullet->HasFocus())
	{
		bullet->SelectItem((int) bd.bullet);
	}

	if (!gravityScale->HasFocus())
	{
		std::stringstream ss;
		ss << bd.gravityScale;
		gravityScale->SetText(ss.str());
	}


}


void PropertyBoxBody::setAngle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(angle->GetText());

		ss >> object->bodyDef.angle;

		object->bodyDef.angle *= b2_pi/180.0f;

		box->GrabFocus();
	}
}


void PropertyBoxBody::setType()
{
	object->bodyDef.type = (b2BodyType)type->GetSelectedItem();
}


void PropertyBoxBody::setActive()
{
	object->bodyDef.active = (bool)active->GetSelectedItem();
}


void PropertyBoxBody::setFixedRotation()
{
	object->bodyDef.fixedRotation = (bool)fixedRotation->GetSelectedItem();
}



void PropertyBoxBody::setBullet()
{
	object->bodyDef.bullet = (bool)bullet->GetSelectedItem();
}



void PropertyBoxBody::setGravityScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(gravityScale->GetText());

		ss >> object->bodyDef.gravityScale;

		box->GrabFocus();
	}
}


void PropertyBoxBody::doAddBox()
{
	b2PolygonShape polyShape;
	polyShape.SetAsBox(1.0,1.0);

	object->shapes.push_back(new b2PolygonShape(polyShape));
	
	b2FixtureDef fixtureDef;

	fixtureDef.shape = object->shapes.back();
	fixtureDef.userData = new int(0);

	object->fixtureDefs.push_back(fixtureDef);

	
	colliders.push_back(new PropertyBoxCollider());
	colliders.back()->setCollider(object, object->fixtureDefs.size()-1);
	colliders.back()->setParent(this);
	colliders.back()->build();
	colliders.back()->update();

	//std::stringstream ss;

	//ss << "Collider" << object->fixtureDefs.size()-1;

	//box->Pack(sfg::Separator::Create());
	//box->Pack(sfg::Label::Create(ss.str()));
	box->Pack(colliders.back()->get());



}


void PropertyBoxBody::doAddCircle()
{
	b2CircleShape circleShape;

	object->shapes.push_back(new b2CircleShape(circleShape));

	b2FixtureDef fixtureDef;

	fixtureDef.shape = object->shapes.back();
	fixtureDef.userData = new int(0);

	object->fixtureDefs.push_back(fixtureDef);


	colliders.push_back(new PropertyBoxCollider());
	colliders.back()->setCollider(object, object->fixtureDefs.size()-1);
	colliders.back()->setParent(this);
	colliders.back()->build();
	colliders.back()->update();

	box->Pack(colliders.back()->get());
}


void PropertyBoxBody::deleteCollider(int id)
{

	for (int i = id; i < object->fixtureDefs.size(); i++)
	{
		colliders[i]->setCollider(object, i - 1);
	}

	delete object->shapes[id];
	delete object->fixtureDefs[id].userData;

	object->shapes.erase(object->shapes.begin() + id);
	object->fixtureDefs.erase(object->fixtureDefs.begin() + id);


	colliders[id]->get()->Show(false);

	colliders[id]->shutdown();
	delete colliders[id];
	
	colliders.erase(colliders.begin() + id);

}

// collider


void PropertyBoxCollider::setCollider(LevelObject* _object, int id)
{
	object = _object;
	colliderId = id;
}

void PropertyBoxCollider::setParent(PropertyBoxBody* _parent)
{
	parent = _parent;
}


void PropertyBoxCollider::shutdown()
{
	delete shape;

}


void PropertyBoxCollider::update()
{

	b2FixtureDef* collider = &object->fixtureDefs[colliderId];

	std::stringstream ss;
	if (!friction->HasFocus())
	{

		ss << collider->friction;
		friction->SetText(ss.str());

		ss.clear();
		ss.str("");

	}

	if (!restitution->HasFocus())
	{

		ss << collider->restitution;
		restitution->SetText(ss.str());

		ss.clear();
		ss.str("");

	}

	if (!restitution->HasFocus())
	{

		ss << collider->density;
		density->SetText(ss.str());

		ss.clear();
		ss.str("");

	}

	if (!userData->HasFocus())
	{
		ss << *((int*)collider->userData);
		userData->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


	if (!isSensor->HasFocus())
	{
		isSensor->SelectItem((int)collider->isSensor);
	}

	if (shape)
		shape->update();

}



void PropertyBoxCollider::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);


	std::stringstream ss;

	ss << "Collider" << colliderId;

	box->Pack(sfg::Separator::Create());
	box->Pack(sfg::Label::Create(ss.str()));




	sfg::Box::Ptr frictionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		frictionBox->Pack(sfg::Label::Create("Friction:"));

		friction = sfg::Entry::Create();
		friction->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCollider::setFriction, this);
		frictionBox->Pack(friction);
	}
	box->Pack(frictionBox);

	sfg::Box::Ptr restitutionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		restitutionBox->Pack(sfg::Label::Create("Restitution:"));

		restitution = sfg::Entry::Create();
		restitution->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCollider::setRestitution, this);
		restitutionBox->Pack(restitution);
	}
	box->Pack(restitutionBox);


	sfg::Box::Ptr densityBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		densityBox->Pack(sfg::Label::Create("Density:"));

		density = sfg::Entry::Create();
		density->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCollider::setDensity, this);
		densityBox->Pack(density);
	}
	box->Pack(densityBox);

	sfg::Box::Ptr userDataBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		userDataBox->Pack(sfg::Label::Create("User-Data:"));

		userData = sfg::Entry::Create();
		userData->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCollider::setUserData, this);
		userDataBox->Pack(userData);
	}
	box->Pack(userDataBox);

	sfg::Box::Ptr isSensorBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		isSensorBox->Pack(sfg::Label::Create("Is Sensor:"));

		isSensor= sfg::ComboBox::Create();

		isSensor->AppendItem("false");
		isSensor->AppendItem("true");
		
		isSensor->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxCollider::setSensor, this);
		isSensorBox->Pack(isSensor);
	}
	box->Pack(isSensorBox);


	switch (object->fixtureDefs[colliderId].shape->GetType())
	{
	case b2Shape::e_polygon:
		{
			PropertyBoxPolygonShape* _shape = new PropertyBoxPolygonShape();

			_shape->setShape(dynamic_cast<b2PolygonShape*>(object->shapes[colliderId]));

			shape = _shape;

			shape->build();

			box->Pack(shape->get());

		}
		break;
	case b2Shape::e_circle:
		{
			PropertyBoxCircleShape* _shape = new PropertyBoxCircleShape();

			_shape->setShape(dynamic_cast<b2CircleShape*>(object->shapes[colliderId]));

			shape = _shape;

			shape->build();


			box->Pack(shape->get());

		}
		break;
	default:
		shape = 0;
	}


	_delete = sfg::Button::Create("Delete");
	_delete->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxCollider::doDelete, this);

	box->Pack(_delete);




}



void PropertyBoxCollider::setFriction()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(friction->GetText());

		ss >> object->fixtureDefs[colliderId].friction;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}

void PropertyBoxCollider::setRestitution()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(restitution->GetText());

		ss >> object->fixtureDefs[colliderId].restitution;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}

void PropertyBoxCollider::setDensity()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(density->GetText());

		ss >> object->fixtureDefs[colliderId].density;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}

void PropertyBoxCollider::setUserData()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(userData->GetText());

		ss >> *((int*)object->fixtureDefs[colliderId].userData);


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}

void PropertyBoxCollider::setSensor()
{
	object->fixtureDefs[colliderId].isSensor = (bool)isSensor->GetSelectedItem();
}


void PropertyBoxCollider::doDelete()
{
	parent->deleteCollider(colliderId);
}


// circleshape


void PropertyBoxCircleShape::setShape(b2CircleShape* _shape)
{
	shape = _shape;
}


void PropertyBoxCircleShape::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr positionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		positionBox->Pack(sfg::Label::Create("Position:"));

		positionX = sfg::Entry::Create();
		positionX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCircleShape::setPosition, this);
		positionBox->Pack(positionX);

		positionY = sfg::Entry::Create();
		positionY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCircleShape::setPosition, this);
		positionBox->Pack(positionY);

	}
	box->Pack(positionBox);

	sfg::Box::Ptr radiusBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		radiusBox->Pack(sfg::Label::Create("Radius:"));

		radius = sfg::Entry::Create();
		radius->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCircleShape::setRadius, this);
		radiusBox->Pack(radius);

	}
	box->Pack(radiusBox);


}

void PropertyBoxCircleShape::update()
{

	std::stringstream ss;
	if (!positionX->HasFocus() && !positionY->HasFocus())
	{
		ss << shape->m_p.x;
		positionX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << shape->m_p.y;
		positionY->SetText(ss.str());
	
		ss.clear();
		ss.str("");


	}
	

	if (!radius->HasFocus())
	{
		ss << shape->m_radius;

		radius->SetText(ss.str());

	}



}



void PropertyBoxCircleShape::setPosition()
{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;


		ss.str(positionX->GetText());
		ss >> shape->m_p.x;
		
		ss.clear();

		ss.str(positionY->GetText());
		ss >> shape->m_p.y;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}

void PropertyBoxCircleShape::setRadius()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(radius->GetText());
		ss >> shape->m_radius;
		

		ss.clear();

		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}



//polygon



void PropertyBoxPolygonShape::setShape(b2PolygonShape* _shape)
{
	shape = _shape;
}


void PropertyBoxPolygonShape::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr positionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		positionBox->Pack(sfg::Label::Create("Position:"));

		positionX = sfg::Entry::Create();
		positionX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPolygonShape::set, this);
		positionBox->Pack(positionX);

		positionY = sfg::Entry::Create();
		positionY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPolygonShape::set, this);
		positionBox->Pack(positionY);

	}
	box->Pack(positionBox);

	sfg::Box::Ptr sizeBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		sizeBox->Pack(sfg::Label::Create("Size:"));

		width = sfg::Entry::Create();
		width->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPolygonShape::set, this);
		sizeBox->Pack(width);

		height = sfg::Entry::Create();
		height->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPolygonShape::set, this);
		sizeBox->Pack(height);

	}
	box->Pack(sizeBox);


	sfg::Box::Ptr angleBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{

		angleBox->Pack(sfg::Label::Create("Angle:"));

		angle = sfg::Entry::Create();
		angle->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPolygonShape::set, this);
		angleBox->Pack(angle);

	}
	box->Pack(angleBox);


}

void PropertyBoxPolygonShape::update()
{

	std::stringstream ss;
	if (!positionX->HasFocus() && !positionY->HasFocus())
	{
		ss << shape->m_centroid.x;
		positionX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << shape->m_centroid.y;
		positionY->SetText(ss.str());
	
		ss.clear();
		ss.str("");


	}
	

	if (!width->HasFocus() && !height->HasFocus())
	{
		ss << MATH::magnitude(MATH::b2ToSf(shape->m_vertices[1] - shape->m_vertices[0])) / 2.0f;
		width->SetText(ss.str());
	
		ss.clear();
		ss.str("");

		ss <<  MATH::magnitude(MATH::b2ToSf(shape->m_vertices[3] - shape->m_vertices[0])) / 2.0f;
		height->SetText(ss.str());
	
		ss.clear();
		ss.str("");

	}


	if (!angle->HasFocus())
	{
		ss << atan2(shape->m_vertices[1].y - shape->m_vertices[0].y, shape->m_vertices[1].x - shape->m_vertices[0].x) * 180.0f/b2_pi;
		angle->SetText(ss.str());

		ss.clear();
		ss.str("");


	}

}


void PropertyBoxPolygonShape::set()
{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		b2Vec2 p;
		float hx, hy;
		float _angle;

		ss.str(positionX->GetText());
		ss >> p.x;
		
		ss.clear();

		ss.str(positionY->GetText());
		ss >> p.y;

		ss.clear();

		ss.str(width->GetText());
		ss >> hx;
		
		ss.clear();

		ss.str(height->GetText());
		ss >> hy;

		ss.clear();

		ss.str(angle->GetText());

		ss >> _angle;

		_angle *= b2_pi / 180.0f;


		//float hx = (shape->m_vertices[1].x - shape->m_vertices[0].x) / 2.0f;
		//float hy = (shape->m_vertices[2].y - shape->m_vertices[0].y) / 2.0f;


		shape->SetAsBox( hx, hy, p, _angle);

		//shape->m_centroid.SetZero();

		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}
//
//void PropertyBoxPolygonShape::setSize()
//{
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
//	{
//		std::stringstream ss;
//
//		float hx, hy;
//
//		ss.str(width->GetText());
//		ss >> hx;
//		
//		ss.clear();
//
//		ss.str(height->GetText());
//		ss >> hy;
//
//		b2Vec2 p = shape->m_centroid;
//
//		shape->SetAsBox(hx, hy, p, 0.0f);
//
//		//shape->m_vertices[0].Set(-hx, -hy);
//		//shape->m_vertices[1].Set( hx, -hy);
//		//shape->m_vertices[2].Set( hx,  hy);
//		//shape->m_vertices[3].Set(-hx,  hy);
//		//
//
//		//b2Transform xf;
//		//xf.p = shape->m_centroid;
//		//xf.q.Set(0.0);
//
//		//for (int i = 0; i < 4; ++i)
//		//{
//		//	shape->m_vertices[i] = b2Mul(xf, shape->m_vertices[i]);
//		//	shape->m_normals[i] = b2Mul(xf.q, shape->m_normals[i]);
//		//}
//
//
//		box->GrabFocus();
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
//		box->GrabFocus();
//}
//
//
