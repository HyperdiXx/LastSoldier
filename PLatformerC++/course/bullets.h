#pragma once
#include "libra.h"

const float PLAYER_BULLET_SPEED = 700;
const float CLOWN_BULLET_SPEED = 500;
const float BOSS_BULLET_SPEED = 300;
const sf::Vector2f BULLET_SIZE = { 14, 11 };

const float MAX_WEAPON_COLDOWN = 1;
const float PISTOL_COLDOWN = 0.4f;
const float SHOOTGUN_COLDOWN = 0.5f;
const float AK_COLDOWN = 0.1f;

const float BULLET_VERTICAL_MARGIN = -50;

enum struct BulletType
{
	PLAYER_AK,
	PLAYER_SHOOTGUN,
	DRAGON_BULLET,
};

class Bullet
{
public:
	Bullet(sf::Vector2f const& startPos, int demage, int orientation, float maxRange, BulletType const& type);
	~Bullet() {};
	sf::RectangleShape bodyShape;
	sf::Texture bodyTexture;
	sf::FloatRect collisionRect;
	float currentRange = 0;
	float maxRange;
	float speed;
	BulletType type;
	int movmentOrientation;
	int damage;
	bool isLive = true;

	void Update(float elapsedTime);
};