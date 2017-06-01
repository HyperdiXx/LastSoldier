#pragma once
#include "Hero.h"
#include <SFML\Graphics.hpp>

const sf::Vector2f PLAYER_SIZE = { 53, 75 };//53,75
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;//250

const int PLAYER_START_HEALTH = 1000;
const int PLAYER_START_SHOOTGUN_AMMO = 7;
const int PLAYER_START_AK_AMMO = 60;
const int PLAYER_MAX_AMMO = 50;
const int PLAYER_PISTOL_DAMAGE = 8;
const int PLAYER_AK_DAMAGE = 5;
const int PLAYER_SHOOTGUN_DAMAGE = 7;
const float PLAYER_START_SHOOT_RANGE = 420;

const int PLAYER_MAX_AK_DAMAGE = 16;
const int PLAYER_MAX_SHOOTGUN_DAMAGE = 16;
const int PLAYER_MAX_HEALTH = 240;

const float INJURED_DURATION = 1.0;
const sf::Color INJURED_COLOR = sf::Color(255, 255, 255, 140);
const float GAME_OVER_DURATION = 2;
const float DEAD_ROTATION = 90;

const sf::Vector2f PLAYER_SHOOT_MARGIN = { 0 , 10 };

enum Weapon
{
	PISTOL,
	SHOOTGUN,
	AK,
};

class Player : public Hero
{
public:
	Player() = default;
	~Player() {};

	Weapon currentWeapon = Weapon::PISTOL;
	std::map<Weapon, int> ammoMap;
	int akDamage;
	int shootgunDamage;
	int boxes;
	float injuredColdown;
	std::map<Weapon, float> weaponColdownsMap;
	float switchWeaponColdown;

	int copy_ak_damage;
	int copy_shootgun_damage;
	int copy_ak_ammo;
	int copy_shootgun_ammo;
	int copy_maxHealth;
	int copy_health;
	int copy_boxes;

	bool InitPlayer();

	void SwitchWeapon();

	void Attack();
	void PistolFire(int orientation);
	void ShootgunFire(int orientation);
	void AkFire(int orientation);

	void RotateDeadBody(float elapsedTime);
	void CreateCopy();
	void ReturnCopy();

	void UpdateStatuses();
	void UpdateTexture(float elapsedTime) override;

	void Clear();
};