#include "myMath.h"
#include "Defines.h"
#include <math.h>

namespace MATH
{
	sf::Vector2f b2ToSf(const b2Vec2& rhs)
	{
		return sf::Vector2f(rhs.x, rhs.y);
	}

	b2Vec2 sfTob2(const sf::Vector2f& rhs)
	{
		return b2Vec2(rhs.x, rhs.y);
	}



	sf::Vector2f vMtoPx(const sf::Vector2f& rhs)
	{
		return sf::Vector2f( rhs.x * mToPx, rhs.y * -mToPx);
	}

	sf::Vector2f vPxtoM(const sf::Vector2f& rhs)
	{
		return sf::Vector2f( rhs.x / mToPx, rhs.y / -mToPx);
	}


	float magnitude(const sf::Vector2f& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	float magnitudeSquared(const sf::Vector2f& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	sf::Vector2f normalize(const sf::Vector2f& v)
	{
		float mag = magnitude(v);
		return sf::Vector2f(v.x / mag, v.y / mag);
	}

	float getRandom(int x, int y, int z, int t, int seed)
	{
		x = x&255;
		y = y&255;
		z = z&255;
		t = t&255;

		seed = seed&255;

		return (perm[x + perm[y + perm[z + + perm[t +perm[seed]]]]] % 12 / 12.0f * 2.0f) - 1.0f;

	}



	float noise(sf::Vector3f pos, float t, int seed)
	{

		int floorX = floor(pos.x);
		int floorY = floor(pos.y);
		int floorZ = floor(pos.z);
		int floorT = floor(t);

		float fractX = pos.x - floorX;
		float fractY = pos.y - floorY;
		float fractZ = pos.z - floorZ;
		float fractT = t - floorT;



		float s11 = getRandom(floorX		, floorY		, floorZ		, floorT		, seed);
		float t11 = getRandom(floorX + 1	, floorY		, floorZ		, floorT		, seed);
		float u11 = getRandom(floorX		, floorY + 1	, floorZ		, floorT		, seed);
		float v11 = getRandom(floorX +1		, floorY + 1	, floorZ		, floorT		, seed);

		float s12 = getRandom(floorX		, floorY		, floorZ + 1	, floorT		, seed);
		float t12 = getRandom(floorX + 1	, floorY		, floorZ + 1	, floorT		, seed);
		float u12 = getRandom(floorX		, floorY + 1	, floorZ + 1	, floorT		, seed);
		float v12 = getRandom(floorX +1		, floorY + 1	, floorZ + 1	, floorT		, seed);

		float i111 = s11 * (1.0f - fractX) + t11 * fractX;
		float i112 = u11 * (1.0f - fractX) + v11 * fractX;
		float i113 = i111 * (1.0f - fractY) + i112 * fractY;

		float i121 = s12 * (1.0f - fractX) + t12 * fractX;
		float i122 = u12 * (1.0f - fractX) + v12 * fractX;
		float i123 = i121 * (1.0f - fractY) + i122 * fractY;

		float i1 =  i113 * (1.0f - fractZ) + i123 * fractZ;


		float s21 = getRandom(floorX		, floorY		, floorZ		, floorT + 1	, seed);
		float t21 = getRandom(floorX + 1	, floorY		, floorZ		, floorT + 1	, seed);
		float u21 = getRandom(floorX		, floorY + 1	, floorZ		, floorT + 1	, seed);
		float v21 = getRandom(floorX +1		, floorY + 1	, floorZ		, floorT + 1	, seed);

		float s22 = getRandom(floorX		, floorY		, floorZ + 1	, floorT + 1	, seed);
		float t22 = getRandom(floorX + 1	, floorY		, floorZ + 1	, floorT + 1	, seed);
		float u22 = getRandom(floorX		, floorY + 1	, floorZ + 1	, floorT + 1	, seed);
		float v22 = getRandom(floorX +1		, floorY + 1	, floorZ + 1	, floorT + 1	, seed);

		float i211 = s21 * (1.0f - fractX) + t21 * fractX;
		float i212 = u21 * (1.0f - fractX) + v21 * fractX;
		float i213 = i211 * (1.0f - fractY) + i212 * fractY;

		float i221 = s22 * (1.0f - fractX) + t22 * fractX;
		float i222 = u22 * (1.0f - fractX) + v22 * fractX;
		float i223 = i221 * (1.0f - fractY) + i222 * fractY;

		float i2 =  i213 * (1.0f - fractZ) + i223 * fractZ;

		return i1 * (1.0 - fractT) + i2 * fractT;
	}


	float clamp(float a, float min, float max)
	{
		if (a > max) a = max;
		if (a < min) a = min;

		return a;
	}

}

