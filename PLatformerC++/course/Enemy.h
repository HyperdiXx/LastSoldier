#pragma once
#include "libra.h"
#include "Hero.h"
#include "bullets.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };

const sf::Vector2f ROCK_SIZE = { 50, 100 };
const int ROCK_START_HEALTH = 60;
const float ROCK_MOVE_SPEED = 180;
const float ROCK_PURSUITE_MOVE_SPEED = 300;
const float ROCK_MOVE_SPEED_RANDOM = 80;
const int ROCK_TOUCH_DAMAGE = 25;
const sf::Vector2f ROCK_TARGET_AREA_SIZE = { 600.0f, 120.0f };
const sf::Vector2f ROCK_TARGET_AREA_MARGIN = { 0, -ROCK_SIZE.y };

const sf::Vector2f DRAGON_SIZE = { 55, 100 };
const int DRAGON_START_HEALTH = 60;
const float DRAGON_MOVE_SPEED = 0;
const int DRAGON_SHOOT_DEMAGE = 6;
const int DRAGON_TOUCH_DEMAGE = 10;
const float DRAGON_TARGET_RANGE = 500;
const sf::Vector2f DRAGON_TARGET_AREA_SIZE = { DRAGON_TARGET_RANGE * 2.0f , DRAGON_SIZE.y / 2.0f };
const float DRAGON_SHOOT_RANGE = DRAGON_TARGET_RANGE;
const int DRAGON_BULLET_DAMAGE = 3;
const float DRAGON_SHOOT_COLDOWN = 0.24f;

const sf::Vector2f BIRD_SIZE = { 80, 70 };
const int BIRD_START_HEALTH = 60;
const float BIRD_TARGET_RANGE = 600;
const float BIRD_MIN_TARGET_RANGE = 1;
const float BIRD_MOVE_SPEED = 110;
const int BIRD_TOUCH_DAMAGE = 6;

const sf::Vector2f SPIDER_SIZE = { 50, 50 };
const int SPIDER_START_HEALTH = 60;
const sf::Vector2f SPIDER_TARGET_AREA_SIZE = { 60, 230 };
const float SPIDER_MOVE_SPEED = 100;
const float SIDER_JUMP_SPEED = 200;
const int SPIDER_TOUCH_DAMAGE = 9;


const float WALL_ATTACK_BULLET_MARGIN = 40;
const int BULLETS_IN_WALL_ATTACK_COUNT = 5;
const int WALL_ATTACK_WINDOW_BULLETS_COUNT = 3;

const int LAVA_DEMAGE = 20;
const float BOTTOM_HAND_MARGIN = 10;

const int EASY_DAMAGE_INCREASE = -2;
const int NORMAL_DAMAGE_INCREASE = 0;
const int HARD_DAMAGE_INCREASE = 3;

enum struct EnemyType
{
	ROCK,
	DRAGON,
	BIRD,
	SPIDER,
};

enum struct EnemyActivity
{
	IDLE,
	PURSUIT,
};

class Enemy : public Hero
{
	
public:
	Enemy() = default;
	Enemy(sf::Vector2f const& posRect, EnemyType const& type, int demageIncrease);

	~Enemy() {};
	void CreateRock(int damageIncrease);
	void CreateDragon(int damageIncrease);
	void CreateBird(int damageIncrease);
	void CreateSpider(int damageIncrease);

	sf::Vector2f position;
	sf::FloatRect targetArea;

	sf::Vector2f ghostMove;

	MovementStatus currentRunStatus;

	sf::FloatRect handLeftMiddle;
	sf::FloatRect handLeftBottom;

	sf::FloatRect handRightMiddle;
	sf::FloatRect handRightBottom;

	EnemyType enemyType;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void(Hero const& player)> UpdateActivityStatus;
	std::function<void(float elapsedTime, std::vector<TmxObject> const& blocks)> Idle;
	std::function<void(Hero const& player, std::vector<Bullet*>& bullets, std::vector<TmxObject> const& blocks, float elapsedTime)> Pursuit;

	void UpdateAI(float elapsedTime, Hero const& player, std::vector<TmxObject> const& blocks, std::vector<Bullet*>& bullets);
	void UpdateHands();
	void UpdateTexture(float elapsedTime)override;

	
	void UpdateRockActivityStatus(Hero const& player);
	void UpdateDragonActivityStatus(Hero const& player);
	void UpdateSpiderActivityStatus(Hero const& player);
	void UpdateBirdActivityStatus(Hero const& player);

	void RockWalk(float elapsedTime, std::vector<TmxObject> const& blocks);

	void DragonShoot(std::vector<Bullet*>& bullets);
	bool IsClownOnGround = false;

	void UpdateSpiderPos(float elapsedTime);
	void SpiderPursuite(Hero const& player, std::vector<TmxObject> const& blocks);

	void UpdateBirdPos(float elapsedTime);
	void BirdIdle(float elapsedTime);
	void BirdPursuite(Hero const& player);

	
};