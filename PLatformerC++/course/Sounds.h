#pragma once
#include "libra.h"

const float MAIN_MENU_VOLUME = 10.0f;
const float GAMEPLAY_VOLUME = 30.0f;
const float MENU_BUTTON_SWITCH_VOLUME = 5.0f;
const float WIN_VOLUME = 26.0f;

const float BIRD_DEATH_VOLUME = 50.0f;

const float PLAYER_DEATH_VOLUME = 20.0f;
const float WEAPON_VOLUME = 8.0f;
const float SWITCH_WEAPON_VOLUME = 8.0f;

const float ADD_AMMO_BONUS_VOLUME = 15.0f;
const float ADD_HEALTH_BONUS_VOLUME = 36.0f;

const float COLLIDES_VOLIME = 16.0f;

enum
{
	// Enemy types
	ROCK = 0,
	DRAGON,
	BIRD,
	SPIDER,
	// Bonus types
	AK_AMMO = 0,
	SHOOTGUN_AMMO,
	HEALTH,
	RANDOM,
	BOX,
};

class GameSound
{
public:
	GameSound();
	~GameSound() {};
	sf::Music* currentMusic = nullptr;

	sf::Music menuSound;
	sf::Music level_0_ost;
	sf::Music level_1_ost;
	sf::Music level_2_ost;
	sf::Music gameOverSound;
	sf::Music winSound;

	sf::Music menuButtonSwitchSound;

	sf::Music deathRock;
	sf::Music deathSpider;
	sf::Music deathDragon;
	sf::Music deathBird;

	sf::Music rockCollideSound;
	sf::Music birdCollideSound;

	sf::Music ammoBonusSound;
	sf::Music healthBonusSound;
	sf::Music boxBonusSound;
	sf::Music randomBonusSound;

	void CollideWithEnemy(int type);
	void CollideWithBonus(int type);
	void EnemyDeath(int type);
};

class HeroSound
{
public:
	HeroSound() {};
	~HeroSound() {};
	sf::Music playerHurtGrunt;
	sf::Music playerDeath;

	sf::Music weaponPistol;
	sf::Music weaponAK;
	sf::Music weaponShootgun;
	sf::Music switchWeapon;
	sf::Music emptyWeapon;

	bool InitCharacterSound();
};

class BonusesSound
{
public:
	BonusesSound() {};
	~BonusesSound() {};
	bool InitBonusesSound();
};

void PlayWithoutDouble(sf::Music& audio);