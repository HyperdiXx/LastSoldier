#include "libra.h"
#include "Effects.h"


Blood::Blood(sf::Vector2f const& characterPos, sf::Vector2f const& bulletPos)
{
	bloodTexture.loadFromFile("resources/blood.png");

	positionOffset = characterPos - bulletPos;

	blood.setSize(BLOOD_SIZE);
	blood.setOrigin(0, BLOOD_SIZE.y / 2.0f);
	blood.setTextureRect(sf::IntRect({ 0, 0 }, static_cast<sf::Vector2i>(BLOOD_SIZE)));
	blood.setTexture(&bloodTexture);

	int randomAngle = (rand() % (2 * BLOOD_MAX_ROTATION)) - BLOOD_MAX_ROTATION;
	float rotation = static_cast<float>(randomAngle);

	if (characterPos.x > bulletPos.x)
	{ // character to the left
		rotation += 180;
		positionOffset.x -= BLOOD_MARGIN_X;
	}
	else
	{ // character to the right
		positionOffset.x += BLOOD_MARGIN_X;
	}
	positionOffset.y -= BLOOD_MARGIN_Y;

	blood.setRotation(rotation);

	duration = 0;
	currentFrame = 0;
	secondsPerFrame = BLOOD_DURATION / BLOOD_FRAMES;
}

void Blood::Update(sf::Vector2f const& characterPos, float elapsedTime)
{
	duration += elapsedTime;
	currentFrame = static_cast<int>(duration / secondsPerFrame);

	const int FRAME_POS_X = static_cast<int>(BLOOD_SIZE.x * currentFrame);
	const sf::IntRect frame({ FRAME_POS_X, 0 }, static_cast<sf::Vector2i>(BLOOD_SIZE));
	blood.setTextureRect(frame);
	blood.setPosition(characterPos - positionOffset);
}

void Blood::Draw(sf::RenderWindow& window)
{
	window.draw(blood);
}

GunFire::GunFire()
{
	gunFireTexture.loadFromFile("resources/gunFire.png");

	gunFire.setSize(GUNFIRE_SIZE);
	gunFire.setOrigin(0, GUNFIRE_SIZE.y / 2.0f);
	gunFire.setTextureRect(sf::IntRect({ 0, 0 }, static_cast<sf::Vector2i>(GUNFIRE_SIZE)));
	gunFire.setTexture(&gunFireTexture);

	duration = 0;
	currentFrame = 0;
	secondsPerFrame = GUNFIRE_DURATION / GUNFIRE_FRAMES;
}

void GunFire::Update(sf::Vector2f const& characterPos, float elapsedTime, int orientation)
{
	float firePosX = 0;
	float firePosY = characterPos.y - GUNFIRE_MARGIN_Y;

	if (orientation == LEFT_ORIENTATION)
	{
		gunFire.setRotation(180);
		firePosX = characterPos.x - GUNFIRE_MARGIN_X;
	}
	else if (orientation == RIGHT_ORIENTATION)
	{
		gunFire.setRotation(0);
		firePosX = characterPos.x + GUNFIRE_MARGIN_X;
	}

	duration += elapsedTime;
	currentFrame = static_cast<int>(duration / secondsPerFrame);

	const int FRAME_POS_X = static_cast<int>(GUNFIRE_SIZE.x * currentFrame);
	const sf::IntRect frame({ FRAME_POS_X, 0 }, static_cast<sf::Vector2i>(GUNFIRE_SIZE));
	gunFire.setTextureRect(frame);

	gunFire.setPosition(firePosX, firePosY);
}

void GunFire::Draw(sf::RenderWindow& window)
{
	window.draw(gunFire);
}

Explosion::Explosion(sf::Vector2f const& position)
{
	explosionTexture.loadFromFile("resources/explosion.png");

	explosion.setSize(EXPLOSION_SIZE);
	explosion.setOrigin(EXPLOSION_SIZE.x / 2.0f, EXPLOSION_SIZE.y / 2.0f);
	explosion.setPosition(position + sf::Vector2f(0, EXPLOSION_MARGIN));
	explosion.setTextureRect(sf::IntRect({ 0, 0 }, static_cast<sf::Vector2i>(EXPLOSION_SIZE)));
	explosion.setTexture(&explosionTexture);

	duration = 0;
	currentFrame = 0;
	secondsPerFrame = EXPLOSION_DURATION / EXPLOSION_FRAMES;
}

void Explosion::Update(float elapsedTime)
{
	duration += elapsedTime;
	currentFrame = static_cast<int>(duration / secondsPerFrame);

	const int FRAME_POS_X = static_cast<int>(EXPLOSION_SIZE.x * currentFrame);
	const sf::IntRect frame({ FRAME_POS_X, 0 }, static_cast<sf::Vector2i>(EXPLOSION_SIZE));
	explosion.setTextureRect(frame);
}

void Explosion::Draw(sf::RenderWindow& window)
{
	window.draw(explosion);
}