#include "bullets.h"

using namespace std;
using namespace sf;


Bullet::Bullet(sf::Vector2f const& startPos, int demage, int orientation, float maxRange, BulletType const& type)
{
	switch (type)
	{
	case BulletType::PLAYER_AK:
		bodyTexture.loadFromFile("resources/bullets_player_ak.png");
		speed = PLAYER_BULLET_SPEED;
		break;
	case BulletType::PLAYER_SHOOTGUN:
		bodyTexture.loadFromFile("resources/bullets_player_shootgun.png");
		speed = PLAYER_BULLET_SPEED;
		break;
	case BulletType::DRAGON_BULLET:
		bodyTexture.loadFromFile("resources/bullets_dragon.png");
		speed = CLOWN_BULLET_SPEED;
		break;
	default:
		break;
	}

	collisionRect.left = startPos.x;
	collisionRect.top = startPos.y + BULLET_VERTICAL_MARGIN;
	collisionRect.width = BULLET_SIZE.x;
	collisionRect.height = BULLET_SIZE.y;

	bodyShape.setTexture(&bodyTexture);
	bodyShape.setSize(BULLET_SIZE);
	bodyShape.setPosition({ collisionRect.left, collisionRect.top });
	movmentOrientation = orientation;

	this->type = type;
	this->maxRange = maxRange;
	this->damage = demage;
}

void Bullet::Update(float elapsedTime)
{
	float movementX = speed * elapsedTime;
	float movementY = 0;

	if (movmentOrientation == 1)
	{
		collisionRect.left -= movementX;
	}
	else
	{
		collisionRect.left += movementX;
	}

	collisionRect.top -= movementY;

	bodyShape.setPosition(sf::Vector2f{ collisionRect.left, collisionRect.top });
	currentRange += movementX;
	if (currentRange >= maxRange)
	{
		isLive = false;
	}
}