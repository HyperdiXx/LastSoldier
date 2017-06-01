#include "bonus.h"

using namespace std;
using namespace sf;

Bonus::Bonus(sf::Vector2f const& position, BonusType const& type)
	:bonusType(type)
{
	if (bonusType == BonusType::RANDOMIZE)
	{
		bonusType = BonusType(rand() % BONUS_COUNT);
	}

	bodyShape.setSize(BONUS_BODY_SIZE);

	switch (bonusType)
	{
	case BonusType::AK_AMMO:
		bonusTexture.loadFromFile("resources/bonus_ammo.png");
		break;
	case BonusType::SHOOTGUN_AMMO:
		bonusTexture.loadFromFile("resources/bonus_ammo.png");
		break;
	case BonusType::HEALTH:
		bonusTexture.loadFromFile("resources/bonus_hp.png");
		break;
	case BonusType::GIFT:
		bonusTexture.loadFromFile("resources/bonus_random.png");
		break;
	case BonusType::ITEM_BOX:
		bonusTexture.loadFromFile("files/images/cd.png");
		bodyShape.setSize(ITEM_BOX_SIZE);
		break;
	default:
		break;
	}

	bodyShape.setTexture(&bonusTexture);

	const sf::Vector2f POSITION(position.x, position.y - BONUS_BODY_SIZE.y);
	const sf::Vector2f SIZE = bodyShape.getSize() * 0.5f;
	collisionRect = sf::FloatRect(POSITION, SIZE);

	InitBonusesSound();
}

void Bonus::Update(float elapsedTime, vector<TmxObject> const& blocks)
{
	if (!IsBonusOnGround)
	{
		float rectCenterX = collisionRect.left - collisionRect.width / 2.0f;
		float rectCenterY = collisionRect.top - collisionRect.height / 2.0f;

		bodyShape.setPosition({ rectCenterX , rectCenterY });

		for (auto block : blocks)
		{
			if (block.rect.intersects(collisionRect))
			{
				IsBonusOnGround = true;
			}
		}

		if (!IsBonusOnGround)
		{
			float movement = fallSpeed;

			fallSpeed = fallSpeed + G * elapsedTime;
			movement = fallSpeed * elapsedTime;
			collisionRect.top += movement;
		}
	}
}

void DropBonusFromEnemy(sf::Vector2f const& position, vector<Bonus*>& bonuses, int probability)
{
	int digit = rand() % 100;

	if (digit < probability)
	{
		bonuses.push_back(new Bonus(position));
	}
}

bool Bonus::AddBonusEffect(Player& player)
{
	int* ammo_AK = &player.ammoMap.find(Weapon::AK)->second;
	int* ammo_SHOOTGUN = &player.ammoMap.find(Weapon::SHOOTGUN)->second;

	switch (bonusType)
	{
	case BonusType::AK_AMMO:
		if (!AddPropertyValue(*ammo_AK, BONUS_AK_AMMO_COUNT, PLAYER_MAX_AMMO))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_AK_AMMO_COUNT) + " AK AMMO";
		break;

	case BonusType::SHOOTGUN_AMMO:
		if (!AddPropertyValue(*ammo_SHOOTGUN, BONUS_SHOOTGUN_AMMO_COUNT, PLAYER_MAX_AMMO))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_SHOOTGUN_AMMO_COUNT) + " SHOOTGUN AMMO";
		break;

	case BonusType::HEALTH:
		if (!AddPropertyValue(player.health, BONUS_HEALTH_COUNT, player.maxHealth))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_HEALTH_COUNT) + "HEALTH";
		break;

	case BonusType::ITEM_BOX:
		player.boxes++;
		announcementText = "YOU FOUND ONE BOX!";
		break;

	case BonusType::GIFT:
		AddRandomBonus(player);
		break;
	default:
		break;
	}

	return true;
}

void Bonus::AddRandomBonus(Player& player)
{
	int randomProperty = rand() % 10 + 1;

	if (randomProperty >= 1 && randomProperty <= 2)
	{
		AddPropertyValue(player.akDamage, BONUS_AK_DAMAGE_INCREASE, PLAYER_MAX_AK_DAMAGE);
		announcementText = "+UP AK DAMAGE";
	}
	else if (randomProperty >= 3 && randomProperty <= 4)
	{
		AddPropertyValue(player.shootgunDamage, BONUS_SHOOTGUN_DAMAGE_INCREASE, PLAYER_MAX_SHOOTGUN_DAMAGE);
		announcementText = "+UP SHOOTGUN DAMAGE";
	}
	else if (randomProperty >= 5 && randomProperty <= 6)
	{
		AddPropertyValue(player.maxHealth, BONUS_HEALTH_INCREASE, PLAYER_MAX_HEALTH);
		announcementText = "+UP MAX HEALTH";
	}
	else
	{
		announcementText = "HA-HA-HA, THIS GIFT ARE EMPTY!";
	}
}

void Bonus::DrawBonus(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}

bool AddPropertyValue(int& property, int addedValue, int maxValue)
{
	if (property == maxValue)
	{
		return false;
	}

	while (addedValue > 0 && property < maxValue)
	{
		property++;
		addedValue--;
	}

	return true;
}