#ifndef RAY_CAST_LISTENER
#define RAY_CAST_LISTENR

#include <Box2D\Box2D.h>
#include "GameObject.h"

class RayCastListener : public b2RayCastCallback
{
public:
	RayCastListener()
	{
		fixture = 0;
		type = ZERO;
	}

	float32 ReportFixture(b2Fixture* _fixture, const b2Vec2& _point, const b2Vec2& _normal, float32 _fraction)
	{
		if (type == 0 || *(int*)_fixture->GetUserData() & type)
		{
			fixture = _fixture;
			point = _point;
			normal = _normal;
			fraction = _fraction;

			return fraction;

		}
		else
		{
			return 1.0f;
		}

	}
	
	b2Fixture* fixture;
	b2Vec2 point;
	b2Vec2 normal;
	float32 fraction;

	FixtureType type;

};

#endif