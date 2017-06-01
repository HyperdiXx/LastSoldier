#include "Hero.h"

using namespace std;
using namespace sf;


void Hero::Spawn(sf::Vector2f const& pos)
{
	collisionRect.left = pos.x;
	collisionRect.top = pos.y;
}

void Hero::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -CHARACTERS_JUMP_SPEED;
	}
}

sf::Vector2f Hero::GetCharacterPos() const
{
	sf::Vector2f position(
		collisionRect.left + collisionRect.width / 4.0f,
		collisionRect.top + collisionRect.height);

	return position;
}

void Hero::UpdatePos(float elapsedTime, vector<TmxObject> const& blocks)
{
	if (runStatus != NOT_RUN)
	{
		float movement = moveSpeed * elapsedTime;

		if (runStatus == RUN_LEFT)
		{
			movement = -movement;
		}
		if (jumpStatus == JUMP)
		{
			movement *= JUMPING_SLOWDOWN;
		}

		collisionRect.left += movement;

		if (IsCollidesWithLevel(collisionRect, blocks))
		{
			collisionRect.left -= movement;
		}
	}

	UpdateGravity(elapsedTime, blocks);

	runStatus = NOT_RUN;
	bodyShape.setPosition(GetCharacterPos());
}

void Hero::UpdateGravity(float elapsedTime, vector<TmxObject> const& blocks)
{
	float movementY = jumpSpeed;

	jumpSpeed = jumpSpeed + G * elapsedTime;
	movementY = jumpSpeed * elapsedTime;

	collisionRect.top += movementY;

	if (IsCollidesWithLevel(collisionRect, blocks))
	{
		collisionRect.top -= movementY;

		if (jumpSpeed > CRITICAL_JUMP_SPEED)
		{
			health -= static_cast<int>(jumpSpeed / SPEED_PER_DEMAGE);
		}

		if (movementY > 0)
		{
			jumpStatus = ON_GROUND;
		}
		jumpSpeed = 0;
	}
	else
	{
		jumpStatus = JUMP;
	}
}

void Hero::UpdateHealthStatus()
{
	if (health <= 0)
	{
		existStatus = ExistenceStatus::DEAD;

		if (deathSound != nullptr && deathSound->getStatus() != sf::Music::Playing)
		{
			deathSound->play();
		}
	}
}

bool Hero::IsCollidesWithLevel(FloatRect const& rect, vector<TmxObject> const& blocks)
{
	return any_of(blocks.begin(), blocks.end(), [&](const TmxObject &block) {
		return (rect.intersects(block.rect) && block.name == "solid");
	});
}

void Hero::DrawCharacter(RenderWindow& window)
{
	window.draw(bodyShape);
	for (auto blood : wounds)
	{
		blood->Draw(window);
	}
	for (auto fire : gunFire)
	{
		fire->Draw(window);
	}
}

void Hero::UpdateOrientation()
{
	if (runStatus == MovementStatus::RUN_LEFT)
	{
		orientationStatus = OrientationStatus::LEFT;
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		orientationStatus = OrientationStatus::RIGHT;
	}
}

void Hero::UpdateTexture(float elapsedTime)
{
	(void)elapsedTime;
	auto bodyWidth = static_cast<int>(bodyShape.getSize().x);
	auto bodyHeight = static_cast<int>(bodyShape.getSize().y);

	if (orientationStatus == OrientationStatus::LEFT)
	{
		bodyShape.setTextureRect(sf::IntRect(0, 0, bodyWidth, bodyHeight));
	}
	else
	{
		bodyShape.setTextureRect(sf::IntRect(bodyWidth, 0, bodyWidth, bodyHeight));
	}
}