#include "Player.h"
#include "Sounds.h"
#include "libra.h"
#include "bullets.h"

using namespace sf;
using namespace std;


enum
{
	// AMMO
	INFINITY_AMMO = INT_MAX,
};

bool Player::InitPlayer()
{
	if (!bodyTexture.loadFromFile("files/images/Pplayer.png") ||
		!InitCharacterSound())
	{
		return false;
	}

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;

	bodyShape.setTexture(&bodyTexture);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);
	bodyShape.setRotation(0);

	existStatus = ExistenceStatus::LIVE;
	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	maxHealth = PLAYER_START_HEALTH;
	health = PLAYER_START_HEALTH;
	boxes = 0;
	shootDamage = PLAYER_PISTOL_DAMAGE;
	akDamage = PLAYER_AK_DAMAGE;
	shootgunDamage = PLAYER_SHOOTGUN_DAMAGE;
	shootRange = PLAYER_START_SHOOT_RANGE;
	deathSound = &playerDeath;
	ammoMap = {
		{ Weapon::PISTOL, INFINITY_AMMO },
		{ Weapon::SHOOTGUN, PLAYER_START_SHOOTGUN_AMMO },
		{ Weapon::AK, PLAYER_START_AK_AMMO }
	};

	injuredColdown = INJURED_DURATION;
	weaponColdownsMap = {
		{ Weapon::PISTOL, PISTOL_COLDOWN },
		{ Weapon::SHOOTGUN, SHOOTGUN_COLDOWN },
		{ Weapon::AK, AK_COLDOWN }
	};
	switchWeaponColdown = 0;

	characterBullets.clear();
	CreateCopy();

	return true;
}

void Player::UpdateStatuses()
{
	if (injuredColdown < INJURED_DURATION)
	{
		bodyShape.setFillColor(INJURED_COLOR);
	}
	else
	{
		bodyShape.setFillColor(Color(255, 255, 255, 255));
	}
}

void Player::UpdateTexture(float elapsedTime)
{
	auto bodyWidth = static_cast<int>(bodyShape.getSize().x);
	auto bodyHeight = static_cast<int>(bodyShape.getSize().y);
	sf::Vector2i frameSize(bodyWidth, bodyHeight);

	int weaponMargin = currentWeapon * 3 * bodyHeight;
	int orientationMargin = (orientationStatus == LEFT) ? FRAMES_COUNT * bodyWidth : 0;
	int jumpMargin = (jumpStatus == JUMP) ? 2 * bodyHeight : 0;
	int runMargin = 0;

	if (jumpStatus == ON_GROUND)
	{
		runMargin = (runStatus != NOT_RUN) ? bodyHeight : 0;
	}

	animateTime += elapsedTime;
	int frame = static_cast<int>(animateTime / TIME_TO_FRAME);

	if (frame == FRAMES_COUNT - 1)
	{
		animateTime = 0;
	}

	int verticalFramePos = frame * frameSize.x + orientationMargin;
	int horizontalFramePos = weaponMargin + jumpMargin + runMargin;
	bodyShape.setTextureRect(sf::IntRect({ verticalFramePos, horizontalFramePos }, frameSize));
}

void Player::SwitchWeapon()
{
	switchWeapon.play();
	int currWeapon = static_cast<int>(currentWeapon);

	if (currentWeapon == Weapon::AK)
	{
		currentWeapon = Weapon::PISTOL;
	}
	else
	{
		currentWeapon = Weapon(currWeapon + 1);
	}
}

void Player::Attack()
{
	const float WEAPON_COLDOWN = weaponColdownsMap.find(currentWeapon)->second;

	int rounds = ammoMap.find(currentWeapon)->second;

	bool IsEnoughAmmo = rounds > 0;
	bool IsEmptySoundPlaying = emptyWeapon.getStatus() == sf::Music::Status::Playing;
	bool IsColdownEnded = shootColdown > WEAPON_COLDOWN;

	if (IsColdownEnded)
	{
		if (IsEnoughAmmo)
		{
			switch (currentWeapon)
			{
			case Weapon::PISTOL:
				PistolFire(orientationStatus);
				break;

			case Weapon::SHOOTGUN:
				ShootgunFire(orientationStatus);
				break;

			case Weapon::AK:
				AkFire(orientationStatus);
				break;

			default:
				break;
			}

			gunFire.push_back(new GunFire());
			shootColdown = 0;
		}
		else if (!IsEmptySoundPlaying)
		{
			emptyWeapon.play();
			shootColdown = 0;
		}
	}
}

void Player::PistolFire(int orientation)
{
	weaponPistol.play();

	auto bullPos = GetCharacterPos() + PLAYER_SHOOT_MARGIN;

	auto bullet = new Bullet(bullPos, shootDamage, orientation, shootRange, BulletType::PLAYER_AK);
	characterBullets.push_back(bullet);
}

void Player::ShootgunFire(int orientation)
{
	weaponShootgun.play();

	Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25) + PLAYER_SHOOT_MARGIN;
	Vector2f middleBullPos = GetCharacterPos() + PLAYER_SHOOT_MARGIN;
	Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25) + PLAYER_SHOOT_MARGIN;

	auto topBullet = new Bullet(topBullPos, shootgunDamage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);
	auto midBullet = new Bullet(middleBullPos, shootgunDamage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);
	auto botBullet = new Bullet(bottomBullPos, shootgunDamage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);

	characterBullets.push_back(topBullet);
	characterBullets.push_back(midBullet);
	characterBullets.push_back(botBullet);

	ammoMap.find(currentWeapon)->second--;
}

void Player::AkFire(int orientation)
{
	weaponAK.play();

	auto bullPos = GetCharacterPos() + PLAYER_SHOOT_MARGIN;

	auto bullet = new Bullet(bullPos, akDamage, orientation, shootRange, BulletType::PLAYER_AK);
	characterBullets.push_back(bullet);
	ammoMap.find(currentWeapon)->second--;
}

void Player::RotateDeadBody(float elapsedTime)
{
	bodyShape.rotate(DEAD_ROTATION * elapsedTime / GAME_OVER_DURATION);
}

void Player::CreateCopy()
{
	copy_ak_damage = akDamage;
	copy_shootgun_damage = shootgunDamage;
	copy_ak_ammo = ammoMap.find(Weapon::AK)->second;
	copy_shootgun_ammo = ammoMap.find(Weapon::SHOOTGUN)->second;
	copy_maxHealth = maxHealth;
	copy_health = health;
	copy_boxes = boxes;
}

void Player::ReturnCopy()
{
	akDamage = copy_ak_damage;
	shootgunDamage = copy_shootgun_damage;
	ammoMap.find(Weapon::AK)->second = copy_ak_ammo;
	ammoMap.find(Weapon::SHOOTGUN)->second = copy_shootgun_ammo;
	maxHealth = copy_maxHealth;
	health = maxHealth; // all levels start with max health
	bodyShape.setRotation(0);
	existStatus = ExistenceStatus::LIVE;
	boxes = copy_boxes;
}

void Player::Clear()
{
	boxes = 0;
	health = maxHealth; // all levels start with max health
}