#include "PropertyBoxAi.h"
#include "LevelObject.h"

void PropertyBoxAi::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);
}



//Plattform

void PropertyBoxPlattform::setObject(LevelObject* _object)
{
	object = _object;
}

void PropertyBoxPlattform::setPlattformComponent(PlattformComponent* _ai)
{
	ai = _ai;
}


void PropertyBoxPlattform::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr startValueBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	startValueBox->Pack(sfg::Label::Create("StartValue:"));
	startValue = sfg::Entry::Create();
	startValue->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlattform::setStartValue, this);
	startValueBox->Pack(startValue);

	box->Pack(startValueBox);



	//box->Pack(sfg::Label::Create("Endposition:"),false);

	//sfg::Box::Ptr endpositionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);

	//endpositionX = sfg::Entry::Create();
	//endpositionX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlattform::setEndPosition, this);
	//endpositionY = sfg::Entry::Create();
	//endpositionY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlattform::setEndPosition, this);

	//endpositionBox->Pack(endpositionX);
	//endpositionBox->Pack(endpositionY);

	//box->Pack(endpositionBox);

	sfg::Box::Ptr velocityBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	velocityBox->Pack(sfg::Label::Create("Velocity:"));
	velocity = sfg::Entry::Create();
	velocity->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlattform::setVelocity, this);
	velocityBox->Pack(velocity);

	box->Pack(velocityBox);

	sfg::Box::Ptr addDeleteBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	addWayPoint = sfg::Button::Create("Add WayPoint");
	addWayPoint->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxPlattform::doAddWayPoint, this);
	addDeleteBox->Pack(addWayPoint);

	deleteWayPoint = sfg::Button::Create("Delete WayPoint");
	deleteWayPoint->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxPlattform::doDeleteWayPoint, this);
	addDeleteBox->Pack(deleteWayPoint);

	box->Pack(addDeleteBox);

	wayPoints.reserve(32);
	for (int i = 0; i < ai->getNumWayPoints(); i++)
	{
		wayPoints.push_back(WayPointBox());
		wayPoints.back().id = i;
		wayPoints.back().setPlattformComponent(ai);
		wayPoints.back().build();
		
		box->Pack(wayPoints.back().get());
	}


}

void PropertyBoxPlattform::update()
{
	std::stringstream ss;

	//if (!endpositionX->HasFocus() && !endpositionY->HasFocus())
	//{
	//	ss << ai->getEndPosition().x;

	//	endpositionX->SetText(ss.str());

	//	ss.clear();
	//	ss.str("");

	//	ss << ai->getEndPosition().y;

	//	endpositionY->SetText(ss.str());

	//	ss.clear();
	//	ss.str("");
	//}

	if (!startValue->HasFocus())
	{
		ss << ai->getStartValue();

		startValue->SetText(ss.str());
		
		ss.clear();
		ss.str("");
	}


	if (!velocity->HasFocus())
	{
		ss << ai->getVelocity();

		velocity->SetText(ss.str());
	}

	ai->setStartPosition(object->getPosition());

	for (int i = 0; i < wayPoints.size(); i++)
	{
		wayPoints[i].update();
	}


}

void PropertyBoxPlattform::setStartValue()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _startValue;
		std::stringstream ss;

		ss.str(startValue->GetText());

		ss >> _startValue;

		ai->setStartValue(_startValue);

		box->GrabFocus();

	}

}


//void PropertyBoxPlattform::setEndPosition()
//{
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
//	{
//		sf::Vector2f endPosition;
//		std::stringstream ss;
//
//		ss.str(endpositionX->GetText());
//
//		ss >> endPosition.x;
//
//		ss.clear();
//
//		ss.str(endpositionY->GetText());
//
//		ss >> endPosition.y;
//
//		ai->setEndPosition(endPosition);
//
//		box->GrabFocus();
//
//	}
//
//}

void PropertyBoxPlattform::setVelocity()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _velocity;
		std::stringstream ss;

		ss.str(velocity->GetText());

		ss >> _velocity;

		ai->setVelocity(_velocity);

		box->GrabFocus();

	}

}


void PropertyBoxPlattform::doAddWayPoint()
{
	ai->addWayPoint(sf::Vector2f(0.0f,0.0f));

	wayPoints.push_back(WayPointBox());
	wayPoints.back().id = wayPoints.size()-1;
	wayPoints.back().setPlattformComponent(ai);
	wayPoints.back().build();

	box->Pack(wayPoints.back().get());
}

void PropertyBoxPlattform::doDeleteWayPoint()
{
	ai->deleteWayPoint();

	wayPoints.back().get()->Show(false);

	wayPoints.pop_back();
}



//swing

void PropertyBoxSwing::setSwingComponent(SwingComponent* _ai)
{
	ai = _ai;
}



void PropertyBoxSwing::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr startAngleBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		startAngleBox->Pack(sfg::Label::Create("StartAngle:"));
		startAngle = sfg::Entry::Create();
		startAngle->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxSwing::setStartAngle, this);
		startAngleBox->Pack(startAngle);
	}
	box->Pack(startAngleBox);


	sfg::Box::Ptr maxAngleBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		maxAngleBox->Pack(sfg::Label::Create("MaxAngle:"));
		maxAngle = sfg::Entry::Create();
		maxAngle->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxSwing::setMaxAngle, this);
		maxAngleBox->Pack(maxAngle);
	}
	box->Pack(maxAngleBox);

	sfg::Box::Ptr velocityBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		velocityBox->Pack(sfg::Label::Create("Velocity:"));
		velocity = sfg::Entry::Create();
		velocity->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxSwing::setVelocity, this);
		velocityBox->Pack(velocity);
	}
	box->Pack(velocityBox);
}


void PropertyBoxSwing::update()
{
	std::stringstream ss;

	
	if (!startAngle->HasFocus())
	{
		ss << ai->getStartAngle();
		startAngle->SetText(ss.str());

		ss.str("");
		ss.clear();
	}

	if (!maxAngle->HasFocus())
	{
		ss << ai->getMaxAngle();
		maxAngle->SetText(ss.str());

		ss.str("");
		ss.clear();
	}


	if (!velocity->HasFocus())
	{
		ss << ai->getVelocity();
		velocity->SetText(ss.str());
	}
}


void PropertyBoxSwing::setStartAngle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(startAngle->GetText());

		float angle;

		ss >> angle;

		ai->setStartAngle(angle);

		box->GrabFocus();
	}
}



void PropertyBoxSwing::setMaxAngle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(maxAngle->GetText());

		float angle;

		ss >> angle;

		ai->setMaxAngle(angle);

		box->GrabFocus();
	}
}

void PropertyBoxSwing::setVelocity()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(velocity->GetText());

		float velocity;

		ss >> velocity;

		ai->setVelocity(velocity);
		box->GrabFocus();
	}
}



//Bubble


void PropertyBoxBubble::setBubbleComponent(TimeBubbleComponent* _ai)
{
	ai = _ai;
}



void PropertyBoxBubble::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr lifeTimeBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		lifeTimeBox->Pack(sfg::Label::Create("Lifetime:"));
		lifeTime = sfg::Entry::Create();
		lifeTime->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxBubble::setLifeTime, this);
		lifeTimeBox->Pack(lifeTime);
	}
	box->Pack(lifeTimeBox);


}


void PropertyBoxBubble::update()
{
	std::stringstream ss;

	if (!lifeTime->HasFocus())
	{
		ss << ai->getLifeTime();
		lifeTime->SetText(ss.str());

		ss.str("");
		ss.clear();
	}


}

void PropertyBoxBubble::setLifeTime()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(lifeTime->GetText());

		float _lifeTime;

		ss >> _lifeTime;

		ai->setLifeTime(_lifeTime);

		box->GrabFocus();
	}
}




//jumper



void PropertyBoxJumper::setJumperComponent(JumperComponent* _ai)
{
	ai = _ai;
}



void PropertyBoxJumper::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr maxJumpHeightBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		maxJumpHeightBox->Pack(sfg::Label::Create("MaxJumpHeight:"));
		maxJumpHeight = sfg::Entry::Create();
		maxJumpHeight->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxJumper::setMaxJumpHeight, this);
		maxJumpHeightBox->Pack(maxJumpHeight);
	}
	box->Pack(maxJumpHeightBox);

}


void PropertyBoxJumper::update()
{
	std::stringstream ss;

	if (!maxJumpHeight->HasFocus())
	{
		ss << ai->getMaxJumpHeight();
		maxJumpHeight->SetText(ss.str());
	}

}

void PropertyBoxJumper::setMaxJumpHeight()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(maxJumpHeight->GetText());

		float _maxJumpHeight;

		ss >> _maxJumpHeight;

		ai->setMaxJumpHeight(_maxJumpHeight);

		box->GrabFocus();
	}
}



//Player


void PropertyBoxPlayer::setPlayerComponent(PlayerComponent* _ai)
{
	ai = _ai;
}

void PropertyBoxPlayer::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr numFastBubblesBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		numFastBubblesBox->Pack(sfg::Label::Create("NumFastBubbles:"));
		numFastBubbles = sfg::Entry::Create();
		numFastBubbles->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlayer::setNumBubbles, this);
		numFastBubblesBox->Pack(numFastBubbles);
	}
	box->Pack(numFastBubblesBox);

	sfg::Box::Ptr numSlowBubblesBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		numSlowBubblesBox->Pack(sfg::Label::Create("NumSlowBubbles:"));
		numSlowBubbles = sfg::Entry::Create();
		numSlowBubbles->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxPlayer::setNumBubbles, this);
		numSlowBubblesBox->Pack(numSlowBubbles);
	}
	box->Pack(numSlowBubblesBox);


	sfg::Box::Ptr weaponBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		weaponBox->Pack(sfg::Label::Create("Weapon:"));
		weapon = sfg::ComboBox::Create();
		weapon->AppendItem("Staff");
		weapon->AppendItem("Gun");
		weapon->AppendItem("Bazooka");
		weapon->AppendItem("None");

		weapon->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxPlayer::setWeapon, this);
		weaponBox->Pack(weapon);
	}
	box->Pack(weaponBox);
}


void PropertyBoxPlayer::update()
{

	if (!numFastBubbles->HasFocus())
	{
		std::stringstream ss;

		ss << ai->getNumBubbles().x;

		numFastBubbles->SetText(ss.str());
	}

	if (!numSlowBubbles->HasFocus())
	{
		std::stringstream ss;

		ss << ai->getNumBubbles().y;

		numSlowBubbles->SetText(ss.str());
	}


	if (!weapon->HasFocus())
	{
		weapon->SelectItem(ai->getCurrWeapon() - 1);

	}


}


void PropertyBoxPlayer::setNumBubbles()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		ai->setNumBubbles(atof(numFastBubbles->GetText().toAnsiString().c_str()), atof(numSlowBubbles->GetText().toAnsiString().c_str()));

		box->GrabFocus();
	}




}

void PropertyBoxPlayer::setWeapon()
{
	ai->setCurrWeapon(static_cast<PlayerComponent::Weapon>(weapon->GetSelectedItem() + 1));

}


//Event

void PropertyBoxEvent::setEventComponent(EventComponent* _ai)
{

	ai = _ai;

	for (int i = 0; i < MAX_EVENT; i++)
		events[i].ai = _ai;
}


void PropertyBoxEvent::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	for (int i = 0; i < MAX_EVENT; i++)
	{
		events[i].id = i;


		std::stringstream ss;
		ss << "Event" << i << ":";
		box->Pack(sfg::Label::Create(ss.str()));
		box->Pack(sfg::Separator::Create());


		sfg::Box::Ptr activationBox = sfg::Box::Create();
		activationBox->Pack(sfg::Label::Create("Activation"));

		events[i].activation = sfg::ComboBox::Create();
		events[i].activation->GetSignal(sfg::ComboBox::OnSelect).Connect(&EventBox::setActivation, &events[i]);

		events[i].activation->AppendItem("NEVER");
		events[i].activation->AppendItem("On Collision begin");
		events[i].activation->AppendItem("On Collision end");
		activationBox->Pack(events[i].activation);

		box->Pack(activationBox);

		sfg::Box::Ptr activatorBox = sfg::Box::Create();
		activatorBox->Pack(sfg::Label::Create("Activator"));

		events[i].activator = sfg::Entry::Create();
		events[i].activator->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setActivator, &events[i]);

		activatorBox->Pack(events[i].activator);
		box->Pack(activatorBox);

		sfg::Box::Ptr receiverBox = sfg::Box::Create();
		receiverBox->Pack(sfg::Label::Create("Receiver"));

		events[i].receiver = sfg::Entry::Create();
		events[i].receiver->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setReceiver, &events[i]);

		receiverBox->Pack(events[i].receiver);
		box->Pack(receiverBox);

		sfg::Box::Ptr messageBox = sfg::Box::Create();
		messageBox->Pack(sfg::Label::Create("Event"));

		events[i].message = sfg::Entry::Create();
		events[i].message->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setMessage, &events[i]);

		messageBox->Pack(events[i].message);
		box->Pack(messageBox);

		sfg::Box::Ptr argBox = sfg::Box::Create();

		events[i].arg0 = sfg::Entry::Create();
		events[i].arg0->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setMessage, &events[i]);
		argBox->Pack(events[i].arg0);


		events[i].arg1 = sfg::Entry::Create();
		events[i].arg1->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setMessage, &events[i]);
		argBox->Pack(events[i].arg1);

		events[i].arg2 = sfg::Entry::Create();
		events[i].arg2->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setMessage, &events[i]);
		argBox->Pack(events[i].arg2);

		events[i].arg3 = sfg::Entry::Create();
		events[i].arg3->GetSignal(sfg::Entry::OnKeyPress).Connect(&EventBox::setMessage, &events[i]);
		argBox->Pack(events[i].arg3);

		box->Pack(argBox);

	}


}


void PropertyBoxEvent::update()
{
	for (int i = 0; i < MAX_EVENT; i++)
	{
		events[i].activation->SelectItem(ai->getActivation(i));

		if (!events[i].activator->HasFocus())
		{
			std::stringstream ss;

			ss << ai->getActivator(i);

			events[i].activator->SetText(ss.str());
		}

		if (!events[i].receiver->HasFocus())
		{
			events[i].receiver->SetText(ai->getReceiver(i));
		}

		if (!events[i].message->HasFocus())
		{
			events[i].message->SetText(ai->getMessage(i).getName());
		}

		if (!events[i].arg0->HasFocus())
		{
			std::stringstream ss;

			ss << *(float*)ai->getMessage(i).getArgument(0);

			events[i].arg0->SetText(ss.str());
		}

		if (!events[i].arg1->HasFocus())
		{
			std::stringstream ss;

			ss << *(float*)ai->getMessage(i).getArgument(1);

			events[i].arg1->SetText(ss.str());
		}

		if (!events[i].arg2->HasFocus())
		{
			std::stringstream ss;

			ss << *(float*)ai->getMessage(i).getArgument(2);

			events[i].arg2->SetText(ss.str());
		}


		if (!events[i].arg3->HasFocus())
		{
			std::stringstream ss;

			ss << *(float*)ai->getMessage(i).getArgument(3);

			events[i].arg3->SetText(ss.str());
		}

	}

}




//Enemy




//jumper



void PropertyBoxEnemy::setEnemyComponent(EnemyComponent* _ai)
{
	ai = _ai;
}



void PropertyBoxEnemy::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr maxJumpHeightBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		maxJumpHeightBox->Pack(sfg::Label::Create("MaxJumpHeight:"));
		maxJumpHeight = sfg::Entry::Create();
		maxJumpHeight->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxEnemy::setMaxJumpHeight, this);
		maxJumpHeightBox->Pack(maxJumpHeight);
	}
	box->Pack(maxJumpHeightBox);

	sfg::Box::Ptr jumpIntervalBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		jumpIntervalBox->Pack(sfg::Label::Create("JumpInterval:"));
		jumpInterval = sfg::Entry::Create();
		jumpInterval->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxEnemy::setJumpInterval, this);
		jumpIntervalBox->Pack(jumpInterval);
	}
	box->Pack(jumpIntervalBox);


	sfg::Box::Ptr walkSpeedBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		walkSpeedBox->Pack(sfg::Label::Create("WalkSpeed:"));
		walkSpeed = sfg::Entry::Create();
		walkSpeed->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxEnemy::setWalkSpeed, this);
		walkSpeedBox->Pack(walkSpeed);
	}
	box->Pack(walkSpeedBox);


	sfg::Box::Ptr turnIntervalBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		turnIntervalBox->Pack(sfg::Label::Create("TurnInterval:"));
		turnInterval = sfg::Entry::Create();
		turnInterval->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxEnemy::setTurnInterval, this);
		turnIntervalBox->Pack(turnInterval);
	}
	box->Pack(turnIntervalBox);

}


void PropertyBoxEnemy::update()
{
	std::stringstream ss;

	if (!maxJumpHeight->HasFocus())
	{
		ss << ai->getMaxJumpHeight();
		maxJumpHeight->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!jumpInterval->HasFocus())
	{
		ss << ai->getJumpInterval();
		jumpInterval->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!walkSpeed->HasFocus())
	{
		ss << ai->getWalkSpeed();
		walkSpeed->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!turnInterval->HasFocus())
	{
		ss << ai->getTurnInterval();
		turnInterval->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

}

void PropertyBoxEnemy::setMaxJumpHeight()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(maxJumpHeight->GetText());

		float _maxJumpHeight;

		ss >> _maxJumpHeight;

		ai->setMaxJumpHeight(_maxJumpHeight);

		box->GrabFocus();
	}
}



void PropertyBoxEnemy::setJumpInterval()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(jumpInterval->GetText());

		float _jumpInterval;

		ss >> _jumpInterval;

		ai->setJumpInterval(_jumpInterval);

		box->GrabFocus();
	}
}


void PropertyBoxEnemy::setWalkSpeed()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(walkSpeed->GetText());

		float _walkSpeed;

		ss >> _walkSpeed;

		ai->setWalkSpeed(_walkSpeed);

		box->GrabFocus();
	}
}



void PropertyBoxEnemy::setTurnInterval()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(turnInterval->GetText());

		float _turnInterval;

		ss >> _turnInterval;

		ai->setTurnInterval(_turnInterval);

		box->GrabFocus();
	}
}



//Shroom


void PropertyBoxShroom::setShroomComponent(ShroomComponent* _ai)
{
	ai = _ai;
	enemyBox.setEnemyComponent(ai);
}


void PropertyBoxShroom::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	enemyBox.build();

	box->Pack(enemyBox.get(), false, false);

	sfg::Box::Ptr bounceHeightBox = sfg::Box::Create();
	bounceHeightBox->Pack(sfg::Label::Create("BounceHeight:"));
	bounceHeight = sfg::Entry::Create();
	bounceHeight->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxShroom::setBounceHeight, this);
	bounceHeightBox->Pack(bounceHeight);

	box->Pack(bounceHeightBox);


}



void PropertyBoxShroom::update()
{
	enemyBox.update();


	std::stringstream ss;

	if (!bounceHeight->HasFocus())
	{
		ss << ai->getBounceHeight();
		bounceHeight->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

}


void PropertyBoxShroom::setBounceHeight()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(bounceHeight->GetText());

		float _bounceHeight;

		ss >> _bounceHeight;

		ai->setBounceHeight(_bounceHeight);

		box->GrabFocus();
	}
}



//Particle


void PropertyBoxParticle::setParticleComponent(ParticleComponent* _ai)
{
	ai = _ai;
}


void PropertyBoxParticle::build()
{

	box = sfg::Box::Create(sfg::Box::VERTICAL);
	box->SetSpacing(3.0f);

	sfg::Box::Ptr depthBox = sfg::Box::Create();
	depthBox->Pack(sfg::Label::Create("Depth:"), false,false);
	depth = sfg::Entry::Create();
	depth->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setDepth, this);
	depthBox->Pack(depth);
	box->Pack(depthBox);

	sfg::Box::Ptr maxParticlesBox = sfg::Box::Create();
	maxParticlesBox->Pack(sfg::Label::Create("MaxParticles:"), false,false);
	maxParticles = sfg::Entry::Create();
	maxParticles->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMaxParticles, this);
	maxParticlesBox->Pack(maxParticles);
	box->Pack(maxParticlesBox);

	sfg::Box::Ptr spawnIntervallBox = sfg::Box::Create();
	spawnIntervallBox->Pack(sfg::Label::Create("SpawnIntervall:"), false,false);
	spawnIntervall = sfg::Entry::Create();
	spawnIntervall->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSpawnIntervall, this);
	spawnIntervallBox->Pack(spawnIntervall);
	box->Pack(spawnIntervallBox);


	sfg::Box::Ptr spawnRadiusBox = sfg::Box::Create();
	spawnRadiusBox->Pack(sfg::Label::Create("SpawnRadius:"), false,false);
	spawnRadiusX = sfg::Entry::Create();
	spawnRadiusX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSpawnRadius, this);
	spawnRadiusBox->Pack(spawnRadiusX);
	spawnRadiusY = sfg::Entry::Create();
	spawnRadiusY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSpawnRadius, this);
	spawnRadiusBox->Pack(spawnRadiusY);
	spawnRadiusZ = sfg::Entry::Create();
	spawnRadiusZ->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSpawnRadius, this);
	spawnRadiusBox->Pack(spawnRadiusZ);
	box->Pack(spawnRadiusBox);

	sfg::Box::Ptr spawnCountBox = sfg::Box::Create();
	spawnCountBox->Pack(sfg::Label::Create("StartCount:"), false,false);
	spawnCount = sfg::Entry::Create();
	spawnCount->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSpawnCount, this);
	spawnCountBox->Pack(spawnCount);
	box->Pack(spawnCountBox);

	box->Pack(sfg::Label::Create("Particle"), false,false);
	box->Pack(sfg::Separator::Create());

	sfg::Box::Ptr spriteBox = sfg::Box::Create();
	spriteBox->Pack(sfg::Label::Create("Sprite:"), false,false);
	sprite = sfg::Entry::Create();
	sprite->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setSprite, this);
	spriteBox->Pack(sprite);
	box->Pack(spriteBox);

	sfg::Box::Ptr scaleBox = sfg::Box::Create();
	scaleBox->Pack(sfg::Label::Create("Scale:"), false,false);
	scale = sfg::Entry::Create();
	scale->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setScale, this);
	scaleBox->Pack(scale);
	box->Pack(scaleBox);


	sfg::Box::Ptr lifeTimeBox = sfg::Box::Create();
	lifeTimeBox->Pack(sfg::Label::Create("LifeTime:"), false,false);
	lifeTimeMin = sfg::Entry::Create();
	lifeTimeMin->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setLifeTime, this);
	lifeTimeBox->Pack(lifeTimeMin);
	lifeTimeMax = sfg::Entry::Create();
	lifeTimeMax->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setLifeTime, this);
	lifeTimeBox->Pack(lifeTimeMax);
	box->Pack(lifeTimeBox);


	sfg::Box::Ptr minVelocityBox = sfg::Box::Create();
	minVelocityBox->Pack(sfg::Label::Create("MinVelocity:"), false,false);
	minVelocityX = sfg::Entry::Create();
	minVelocityX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMinVelocity, this);
	minVelocityBox->Pack(minVelocityX);
	minVelocityY = sfg::Entry::Create();
	minVelocityY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMinVelocity, this);
	minVelocityBox->Pack(minVelocityY);
	minVelocityZ = sfg::Entry::Create();
	minVelocityZ->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMinVelocity, this);
	minVelocityBox->Pack(minVelocityZ);
	box->Pack(minVelocityBox);

	sfg::Box::Ptr maxVelocityBox = sfg::Box::Create();
	maxVelocityBox->Pack(sfg::Label::Create("MaxVelocity:"), false,false);
	maxVelocityX = sfg::Entry::Create();
	maxVelocityX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMaxVelocity, this);
	maxVelocityBox->Pack(maxVelocityX);
	maxVelocityY = sfg::Entry::Create();
	maxVelocityY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMaxVelocity, this);
	maxVelocityBox->Pack(maxVelocityY);
	maxVelocityZ = sfg::Entry::Create();
	maxVelocityZ->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setMaxVelocity, this);
	maxVelocityBox->Pack(maxVelocityZ);
	box->Pack(maxVelocityBox);

	box->Pack(sfg::Label::Create("Field"), false,false);
	box->Pack(sfg::Separator::Create());

	sfg::Box::Ptr fieldScaleBox = sfg::Box::Create();
	fieldScaleBox->Pack(sfg::Label::Create("FieldScale:"), false,false);
	fieldScaleX = sfg::Entry::Create();
	fieldScaleX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setFieldScale, this);
	fieldScaleBox->Pack(fieldScaleX);
	fieldScaleY = sfg::Entry::Create();
	fieldScaleY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setFieldScale, this);
	fieldScaleBox->Pack(fieldScaleY);
	fieldScaleZ = sfg::Entry::Create();
	fieldScaleZ->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setFieldScale, this);
	fieldScaleBox->Pack(fieldScaleZ);
	box->Pack(fieldScaleBox);

	sfg::Box::Ptr timeScaleBox = sfg::Box::Create();
	timeScaleBox->Pack(sfg::Label::Create("TimeScale:"), false,false);
	timeScale = sfg::Entry::Create();
	timeScale->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxParticle::setTimeScale, this);
	timeScaleBox->Pack(timeScale);
	box->Pack(timeScaleBox);


}


void PropertyBoxParticle::update()
{


	std::stringstream ss;

	if (!depth->HasFocus())
	{
		ss << ai->getDepth();
		depth->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!maxParticles->HasFocus())
	{
		ss << ai->getMaxParticles();
		maxParticles->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!spawnIntervall->HasFocus())
	{
		ss << ai->getSpawnIntervall();
		spawnIntervall->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


	if (!spawnRadiusX->HasFocus() && !spawnRadiusY->HasFocus() && !spawnRadiusZ->HasFocus())
	{
		sf::Vector3f spawnRadius = ai->getSpawnRadius();

		ss << spawnRadius.x;

		spawnRadiusX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << spawnRadius.y;

		spawnRadiusY->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << spawnRadius.z;

		spawnRadiusZ->SetText(ss.str());

		ss.clear();
		ss.str("");
	}
	
	if (!spawnCount->HasFocus())
	{
		ss << ai->getSpawnCount();
		spawnCount->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!sprite->HasFocus())
	{
		std::string _sprite = ai->getSprite();
		_sprite.erase(0, 4);
		sprite->SetText(_sprite);
	}

	if (!scale->HasFocus())
	{
		ss << ai->getScale();
		scale->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!lifeTimeMin->HasFocus() && !lifeTimeMax->HasFocus())
	{
		ss << ai->getLifeTime().x;
		lifeTimeMin->SetText(ss.str());

		ss.clear();
		ss.str("");


		ss << ai->getLifeTime().y;
		lifeTimeMax->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!minVelocityX->HasFocus() && !minVelocityY->HasFocus() && !minVelocityZ->HasFocus())
	{
		sf::Vector3f minVelocity = ai->getMinVelocity();

		ss << minVelocity.x;

		minVelocityX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << minVelocity.y;

		minVelocityY->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << minVelocity.z;

		minVelocityZ->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!maxVelocityX->HasFocus() && !maxVelocityY->HasFocus() && !maxVelocityZ->HasFocus())
	{
		sf::Vector3f maxVelocity = ai->getMaxVelocity();

		ss << maxVelocity.x;

		maxVelocityX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << maxVelocity.y;

		maxVelocityY->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << maxVelocity.z;

		maxVelocityZ->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


	if (!fieldScaleX->HasFocus() && !fieldScaleY->HasFocus() && !fieldScaleZ->HasFocus())
	{
		sf::Vector3f fieldScale = ai->getFieldScale();

		ss << fieldScale.x;

		fieldScaleX->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << fieldScale.y;

		fieldScaleY->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << fieldScale.z;

		fieldScaleZ->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


	if (!timeScale->HasFocus())
	{
		ss << ai->getTimeScale();
		timeScale->SetText(ss.str());

		ss.clear();
		ss.str("");
	}
}


void PropertyBoxParticle::setDepth()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(depth->GetText());
		float _depth;
		ss >> _depth;
		ai->setDepth(_depth);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setMaxParticles()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(maxParticles->GetText());
		float _maxParticles;
		ss >> _maxParticles;
		ai->setMaxParticles(_maxParticles);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setSpawnIntervall()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(spawnIntervall->GetText());
		float _spawnIntervall;
		ss >> _spawnIntervall;
		ai->setSpawnIntervall(_spawnIntervall);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setSpawnRadius()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		
		sf::Vector3f spawnRadius;

		std::stringstream ss;

		ss.str(spawnRadiusX->GetText());
		ss >> spawnRadius.x;
		ss.clear();
		ss.str(spawnRadiusY->GetText());
		ss >> spawnRadius.y;
		ss.clear();
		ss.str(spawnRadiusZ->GetText());
		ss >> spawnRadius.z;


		ai->setSpawnRadius(spawnRadius);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setSpawnCount()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(spawnCount->GetText());
		float _spawnCount;
		ss >> _spawnCount;
		ai->setSpawnCount(_spawnCount);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setSprite()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		ai->setSprite("res/" + sprite->GetText());
		box->GrabFocus();
	}

}

void PropertyBoxParticle::setScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(scale->GetText());
		float _scale;
		ss >> _scale;
		ai->setScale(_scale);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setLifeTime()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		sf::Vector2f _lifeTime;
		
		std::stringstream ss;

		ss.str(lifeTimeMin->GetText());
		ss >> _lifeTime.x;
		ss.clear();
		ss.str(lifeTimeMax->GetText());
		ss >> _lifeTime.y;

		ai->setLifeTime(_lifeTime);

		box->GrabFocus();
	}
}
void PropertyBoxParticle::setMinVelocity()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector3f minVelocity;

		std::stringstream ss;

		ss.str(minVelocityX->GetText());
		ss >> minVelocity.x;
		ss.clear();
		ss.str(minVelocityY->GetText());
		ss >> minVelocity.y;
		ss.clear();
		ss.str(minVelocityZ->GetText());
		ss >> minVelocity.z;


		ai->setMinVelocity(minVelocity);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setMaxVelocity()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector3f maxVelocity;

		std::stringstream ss;

		ss.str(maxVelocityX->GetText());
		ss >> maxVelocity.x;
		ss.clear();
		ss.str(maxVelocityY->GetText());
		ss >> maxVelocity.y;
		ss.clear();
		ss.str(maxVelocityZ->GetText());
		ss >> maxVelocity.z;


		ai->setMaxVelocity(maxVelocity);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setFieldScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector3f fieldScale;

		std::stringstream ss;

		ss.str(fieldScaleX->GetText());
		ss >> fieldScale.x;
		ss.clear();
		ss.str(fieldScaleY->GetText());
		ss >> fieldScale.y;
		ss.clear();
		ss.str(fieldScaleZ->GetText());
		ss >> fieldScale.z;


		ai->setFieldScale(fieldScale);

		box->GrabFocus();
	}
}

void PropertyBoxParticle::setTimeScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;
		ss.str(timeScale->GetText());
		float _timeScale;
		ss >> _timeScale;
		ai->setTimeScale(_timeScale);

		box->GrabFocus();
	}
}


//Rock

void PropertyBoxRock::setRockComponent(RockComponent* _ai)
{
	ai = _ai;
	enemyBox.setEnemyComponent(ai);
}


void PropertyBoxRock::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	enemyBox.build();

	box->Pack(enemyBox.get(), false, false);

	sfg::Box::Ptr attackRangeBox = sfg::Box::Create();
	attackRangeBox->Pack(sfg::Label::Create("AttackRange:"));
	attackRange = sfg::Entry::Create();
	attackRange->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRock::setAttackRange, this);
	attackRangeBox->Pack(attackRange);

	box->Pack(attackRangeBox);

	sfg::Box::Ptr rageNumberBox = sfg::Box::Create();
	rageNumberBox->Pack(sfg::Label::Create("RageNumber:"));
	rageNumber = sfg::Entry::Create();
	rageNumber->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRock::setRageNumber, this);
	rageNumberBox->Pack(rageNumber);

	box->Pack(rageNumberBox);


}



void PropertyBoxRock::update()
{
	enemyBox.update();


	std::stringstream ss;

	if (!attackRange->HasFocus())
	{
		ss << ai->getAttackRange();
		attackRange->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!rageNumber->HasFocus())
	{
		ss << ai->getRageNumber();
		rageNumber->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


}


void PropertyBoxRock::setAttackRange()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(attackRange->GetText());

		float _attackRange;

		ss >> _attackRange;

		ai->setAttackRange(_attackRange);

		box->GrabFocus();
	}
}


void PropertyBoxRock::setRageNumber()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(rageNumber->GetText());

		float _rageNumber;

		ss >> _rageNumber;

		ai->setRageNumber(_rageNumber);

		box->GrabFocus();
	}
}



//Bomb

void PropertyBoxBomb::setBombComponent(BombComponent* _ai)
{
	ai = _ai;
	enemyBox.setEnemyComponent(ai);
}


void PropertyBoxBomb::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	enemyBox.build();

	box->Pack(enemyBox.get(), false, false);

	sfg::Box::Ptr attackRangeBox = sfg::Box::Create();
	attackRangeBox->Pack(sfg::Label::Create("AttackRange:"));
	attackRange = sfg::Entry::Create();
	attackRange->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxBomb::setAttackRange, this);
	attackRangeBox->Pack(attackRange);

	box->Pack(attackRangeBox);

	sfg::Box::Ptr explosionRadiusBox = sfg::Box::Create();
	explosionRadiusBox->Pack(sfg::Label::Create("ExplosionRadius:"));
	explosionRadius = sfg::Entry::Create();
	explosionRadius->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxBomb::setExplosionRadius, this);
	explosionRadiusBox->Pack(explosionRadius);

	box->Pack(explosionRadiusBox);


}



void PropertyBoxBomb::update()
{
	enemyBox.update();


	std::stringstream ss;

	if (!attackRange->HasFocus())
	{
		ss << ai->getAttackRange();
		attackRange->SetText(ss.str());

		ss.clear();
		ss.str("");
	}

	if (!explosionRadius->HasFocus())
	{
		ss << ai->getExplosionRadius();
		explosionRadius->SetText(ss.str());

		ss.clear();
		ss.str("");
	}


}


void PropertyBoxBomb::setAttackRange()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(attackRange->GetText());

		float _attackRange;

		ss >> _attackRange;

		ai->setAttackRange(_attackRange);

		box->GrabFocus();
	}
}


void PropertyBoxBomb::setExplosionRadius()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(explosionRadius->GetText());

		float _explosionRadius;

		ss >> _explosionRadius;

		ai->setExplosionRadius(_explosionRadius);

		box->GrabFocus();
	}
}

