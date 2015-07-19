

	LevelObject* fastTimeBubble = new LevelObject();
{

	RendererComponent* renderer = new RendererComponent();
	renderer->setSprite(imageManager->getTexture("res\\bubbleFast.png"));


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0.0f;
	bodyDef.position.x = 0.0 ;
	bodyDef.position.y = 0.0;


	TimeBubbleComponent* ai = new TimeBubbleComponent();
	ai->setType(FAST);
	ai->setPlayer(0);
	ai->setOwner(0);
	ai->setLifeTime(1.5f);

	fastTimeBubble->setRendererComponent(renderer);
	fastTimeBubble->setAiComponent(ai);
	fastTimeBubble->hasBody = true;
	fastTimeBubble->bodyDef = bodyDef;

	b2CircleShape* shape = new b2CircleShape();
	shape->m_radius = 2.0f;

	fastTimeBubble->shapes.push_back(shape);

	b2FixtureDef fixtureDef;
	fixtureDef.isSensor = true;
	fixtureDef.shape = shape;
	fixtureDef.userData = new int(FASTBUBBLE);

	fastTimeBubble->fixtureDefs.push_back(fixtureDef);


}
addObject(fastTimeBubble);

fastTimeBubble->setName("fastBubble");

	LevelObject* slowTimeBubble = new LevelObject();
	{

		RendererComponent* renderer = new RendererComponent();
		renderer->setSprite(imageManager->getTexture("res\\bubbleSlow.png"));


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = 0.0f;
		bodyDef.position.x = 0.0 ;
		bodyDef.position.y = 0.0;


		TimeBubbleComponent* ai = new TimeBubbleComponent();
		ai->setType(SLOW);
		ai->setPlayer(0);
		ai->setOwner(0);
		ai->setLifeTime(1.5f);

		slowTimeBubble->setRendererComponent(renderer);
		slowTimeBubble->setAiComponent(ai);
		slowTimeBubble->hasBody = true;
		slowTimeBubble->bodyDef = bodyDef;

		b2CircleShape* shape = new b2CircleShape();
		shape->m_radius = 2.0f;

		slowTimeBubble->shapes.push_back(shape);

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.shape = shape;
		fixtureDef.userData = new int(SLOWBUBBLE);

		slowTimeBubble->fixtureDefs.push_back(fixtureDef);

		
	}
	addObject(slowTimeBubble);

	slowTimeBubble->setName("slowBubble");


	LevelObject* swing = new LevelObject();
{

	swing->setCenter(sf::Vector2f(0.0f, -4.5f));

	RendererComponent* renderer = new RendererComponent();
	renderer->setSprite(imageManager->getTexture("res\\swing.png"));
	renderer->setOrigin(sf::Vector2f(50.0f / mToPx, 18.0f / -mToPx));

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.fixedRotation = false;
	bodyDef.bullet = true;

	SwingComponent* ai = new SwingComponent();
	ai->setMaxAngle(45.0f);
	ai->setVelocity(4.0f);

	swing->setPosition(sf::Vector2f(0.0f,0.0f));
	swing->setRendererComponent(renderer);
	swing->hasBody = true;
	swing->bodyDef = bodyDef;
	swing->setAiComponent(ai);

	b2CircleShape* shape = new b2CircleShape();
	shape->m_radius = 1.0f;
	shape->m_p = b2Vec2(0.0f, -4.5f);

	swing->shapes.push_back(shape);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.userData = new int(DEADLY | DYNAMIC);

	swing->fixtureDefs.push_back(fixtureDef);

}
addObject(swing);

swing->setName("Swing");


LevelObject* plattform = new LevelObject();
{

	RendererComponent* renderer = new RendererComponent();
	renderer->setSprite(imageManager->getTexture("res\\plattform.png"));

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.fixedRotation = true;

	PlattformComponent* ai = new PlattformComponent();
	ai->setParent(plattform);
	ai->setVelocity(4.0f);
	ai->setEndPosition(sf::Vector2f(0.0f, 10.0f));
	ai->setStartPosition(sf::Vector2f(0.0f,0.0f));

	plattform->setPosition(sf::Vector2f(0.0f, 0.0f));
	plattform->setRendererComponent(renderer);
	plattform->hasBody = true;
	plattform->bodyDef = bodyDef;
	plattform->setAiComponent(ai);

	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox(1.0f,0.25f);
	plattform->shapes.push_back(shape);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 2.0f;
	fixtureDef.userData = new int(GROUND | DYNAMIC);

	plattform->fixtureDefs.push_back(fixtureDef);

}
addObject(plattform);

plattform->setName("Plattform");



LevelObject* player = new LevelObject();
{

	//RendererComponent* renderer = new RendererComponent();
	//renderer->setSprite(imageManager->getTexture("box.png"));

	AnimationComponent* renderer = new AnimationComponent();
	renderer->setSpriteSheet(imageManager->getTexture("res\\player_sheet.png"), sf::Vector2i(100,100));


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(8.0f, 5.0f);
	bodyDef.fixedRotation = true;

	PlayerComponent* ai = new PlayerComponent();
	ai->setAnimation(renderer);

	player->setPosition(sf::Vector2f(0.0f, 0.0f));
	player->setRendererComponent(renderer);
	player->hasBody = true;
	player->bodyDef = bodyDef;
	player->setAiComponent(ai);

	{
		b2CircleShape* shape = new b2CircleShape();
		shape->m_radius = 0.5f;
		shape->m_p.Set(0.0f, -0.5f);

		player->shapes.push_back(shape);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.friction = 3.0f;
		fixtureDef.userData = new int(FOOT | DYNAMIC);

		player->fixtureDefs.push_back(fixtureDef);
	}

	{
		b2PolygonShape* shape = new b2PolygonShape();
		shape->SetAsBox(0.5f,0.75f, b2Vec2(0.0f, 0.25f),0);

		player->shapes.push_back(shape);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.friction = 0.1f;
		fixtureDef.userData = new int(DYNAMIC);
		player->fixtureDefs.push_back(fixtureDef);
	}

}
addObject(player);

player->setName("Player");


	LevelObject* jumper = new LevelObject();
	{

	RendererComponent* renderer = new RendererComponent();
	renderer->setSprite(imageManager->getTexture("res\\jumper.png"));

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(0.0f, 0.0f);
	bodyDef.fixedRotation = true;

	JumperComponent* ai = new JumperComponent();
	ai->setParent(jumper);
	ai->setMaxJumpHeight(4.0f);

	jumper->setPosition(sf::Vector2f(0.0f, 0.0f));
	jumper->setRendererComponent(renderer);
	jumper->hasBody = true;
	jumper->bodyDef = bodyDef;
	jumper->setAiComponent(ai);

	{
		b2PolygonShape* shape = new b2PolygonShape();
		shape->SetAsBox(1.0f,95 / (2 * mToPx), b2Vec2(0.0f, 60 / ( 2 * mToPx)), 0.0f);
		jumper->shapes.push_back(shape);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 2.0f;
		fixtureDef.userData = new int(DEADLY | DYNAMIC);

		jumper->fixtureDefs.push_back(fixtureDef);
	}

	{
		b2PolygonShape* shape = new b2PolygonShape();
		shape->SetAsBox(1.0f,95 / (2 * mToPx), b2Vec2(0.0f, -130 / (2 * mToPx)), 0.0f);
		jumper->shapes.push_back(shape);


		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 2.0f;
		fixtureDef.userData = new int(DEADLY | DYNAMIC | FOOT);

		jumper->fixtureDefs.push_back(fixtureDef);
	}

	}
	addObject(jumper);

	jumper->setName("Jumper");
