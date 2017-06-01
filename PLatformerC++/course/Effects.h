#pragma once
#include "libra.h"


const sf::Vector2f BLOOD_SIZE = { 45, 30 };
const int BLOOD_MAX_ROTATION = 30;
const float BLOOD_MARGIN_X = 16;
const float BLOOD_MARGIN_Y = 6;
const int BLOOD_FRAMES = 10;
const float BLOOD_DURATION = 0.25f;

const sf::Vector2f GUNFIRE_SIZE = { 24, 16 };
const float GUNFIRE_MARGIN_X = 25;
const float GUNFIRE_MARGIN_Y = 34;
const int GUNFIRE_FRAMES = 5;
const float GUNFIRE_DURATION = 0.2f;

const sf::Vector2f EXPLOSION_SIZE = { 80, 80 };
const float EXPLOSION_MARGIN = -24;
const int EXPLOSION_FRAMES = 12;
const float EXPLOSION_DURATION = 0.65f;

enum
{
	LEFT_ORIENTATION = 1,
	RIGHT_ORIENTATION,
};

class GunFire
{
public:
	GunFire();
	~GunFire() {};
	sf::RectangleShape gunFire;
	sf::Texture gunFireTexture;

	float secondsPerFrame;
	float duration;
	int currentFrame;

	void Update(sf::Vector2f const& characterPos, float elapsedTime, int orientation);
	void Draw(sf::RenderWindow& window);
};

class Blood
{
public:
	Blood(sf::Vector2f const& characterPos, sf::Vector2f const& bulletPos);
	~Blood() {};
	sf::RectangleShape blood;
	sf::Texture bloodTexture;

	sf::Vector2f positionOffset;

	float secondsPerFrame;
	float duration;
	int currentFrame;

	void Update(sf::Vector2f const& characterPos, float elapsedTime);
	void Draw(sf::RenderWindow& window);
};

class Explosion
{
public:
	Explosion(sf::Vector2f const& position);
	~Explosion() {};
	sf::RectangleShape explosion;
	sf::Texture explosionTexture;

	float secondsPerFrame;
	float duration;
	int currentFrame;

	void Update(float elapsedTime);
	void Draw(sf::RenderWindow& window);
}; 
