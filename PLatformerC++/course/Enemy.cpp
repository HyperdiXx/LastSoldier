#include "Enemy.h"

using namespace std;
using namespace sf;


Enemy::Enemy(sf::Vector2f const& position, EnemyType const& type, int damageIncrease)
{
	switch (type)
	{
	case EnemyType::ROCK:
		this->CreateRock(damageIncrease);
		break;
	case EnemyType::DRAGON:
		this->CreateDragon(damageIncrease);
		break;
	case EnemyType::BIRD:
		this->CreateBird(damageIncrease);
		break;
	case EnemyType::SPIDER:
		this->CreateSpider(damageIncrease);
		break;
	default:
		break;
	}

	currentRunStatus = MovementStatus(rand() % 2);

	bodyShape.setTexture(&bodyTexture);

	const sf::Vector2f BODY_SIZE = bodyShape.getSize();
	const sf::Vector2i BODY_SIZE_INT = static_cast<sf::Vector2i>(BODY_SIZE);

	if (enemyType != EnemyType::SPIDER)
	{
		bodyShape.setTextureRect(sf::IntRect({ 0, 0 }, BODY_SIZE_INT));
	}

	bodyShape.setSize(BODY_SIZE);
	bodyShape.setOrigin(BODY_SIZE.x / 2.0f, BODY_SIZE.y);

	const sf::Vector2f BONE_SIZE(BODY_SIZE.x / 2.0f, BODY_SIZE.y - 10);
	collisionRect = sf::FloatRect(position, BONE_SIZE);
}

void Enemy::CreateRock(int damageIncrease)
{
	enemyType = EnemyType::ROCK;

	bodyTexture.loadFromFile("resources/enemyRock.png");
	bodyShape.setSize(ROCK_SIZE);

	health = ROCK_START_HEALTH;
	touchDamage = ROCK_TOUCH_DAMAGE + damageIncrease;

	float randomSpeed = ROCK_MOVE_SPEED_RANDOM * (rand() % 100 + 1) / 100;
	moveSpeed = ROCK_MOVE_SPEED + randomSpeed;

	UpdateActivityStatus = [&](Hero const& player) {
		UpdateRockActivityStatus(player);
	};

	Pursuit = [&](Hero const& player, vector<Bullet*>& bullets, vector<TmxObject> const& blocks, float elapsedTime) {
		(void)bullets;
		(void)player;
		RockWalk(elapsedTime, blocks);
	};

	Idle = [&](float elapsedTime, vector<TmxObject> const& blocks) {
		RockWalk(elapsedTime, blocks);
	};
}

void Enemy::CreateDragon(int damageIncrease)
{
	enemyType = EnemyType::DRAGON;

	bodyTexture.loadFromFile("files/images/daron.png");
	bodyShape.setSize(DRAGON_SIZE);

	health = DRAGON_START_HEALTH;
	shootDamage = DRAGON_SHOOT_DEMAGE + damageIncrease;
	touchDamage = DRAGON_TOUCH_DEMAGE + damageIncrease;
	shootRange = DRAGON_SHOOT_RANGE;

	UpdateActivityStatus = [&](Hero const& player) {
		UpdateDragonActivityStatus(player);
	};

	Pursuit = [&](Hero const& player, vector<Bullet*>& bullets, vector<TmxObject> const& blocks, float elapsedTime) {
		(void)blocks;
		(void)player;
		(void)elapsedTime;
		DragonShoot(bullets);
	};

	Idle = [&](float elapsedTime, vector<TmxObject> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};
}

void Enemy::CreateBird(int damageIncrease)
{
	enemyType = EnemyType::BIRD;

	bodyTexture.loadFromFile("files/images/bird1.png");
	bodyShape.setSize(BIRD_SIZE);

	health = BIRD_START_HEALTH;
	touchDamage = BIRD_TOUCH_DAMAGE + damageIncrease;
	moveSpeed = BIRD_MOVE_SPEED;

	UpdateActivityStatus = [&](Hero const& player) {
		UpdateBirdActivityStatus(player);
	};

	Pursuit = [&](Hero const& player, vector<Bullet*>& bullets, vector<TmxObject> const& blocks, float elapsedTime) {
		(void)bullets;
		(void)blocks;
		(void)elapsedTime;
		BirdPursuite(player);
	};

	Idle = [&](float elapsedTime, vector<TmxObject> const& blocks) {
		(void)blocks;
		BirdIdle(elapsedTime);
	};
}

void Enemy::CreateSpider(int damageIncrease)
{
	enemyType = EnemyType::SPIDER;

	bodyTexture.loadFromFile("resources/enemySpider.png");
	bodyShape.setSize(SPIDER_SIZE);

	health = SPIDER_START_HEALTH;
	touchDamage = SPIDER_TOUCH_DAMAGE + damageIncrease;
	moveSpeed = SPIDER_MOVE_SPEED;
	jumpSpeed = SIDER_JUMP_SPEED;

	UpdateActivityStatus = [&](Hero const& player) {
		UpdateSpiderActivityStatus(player);
	};

	Pursuit = [&](Hero const& player, vector<Bullet*>& bullets, vector<TmxObject> const& blocks, float elapsedTime) {
		(void)bullets;
		(void)elapsedTime;
		SpiderPursuite(player, blocks);
	};

	Idle = [&](float elapsedTime, vector<TmxObject> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};

	const sf::Vector2i BODY_SIZE = {
		static_cast<int>(bodyShape.getSize().x),
		static_cast<int>(bodyShape.getSize().y)
	};
	bodyShape.setTextureRect(sf::IntRect(0, BODY_SIZE.y, BODY_SIZE.x, BODY_SIZE.y));
}


void Enemy::UpdateAI(float elapsedTime, Hero const& player, vector<TmxObject> const& blocks, vector<Bullet*>& bullets)
{
	UpdateHealthStatus();
	UpdateActivityStatus(player);

	if (activityStatus == EnemyActivity::IDLE)
	{
		Idle(elapsedTime, blocks);
	}
	else
	{
		Pursuit(player, bullets, blocks, elapsedTime);
	}

	UpdateOrientation();

	if (enemyType == EnemyType::BIRD)
	{
		UpdateBirdPos(elapsedTime);
	}
	else if (enemyType == EnemyType::SPIDER && activityStatus == EnemyActivity::IDLE)
	{
		UpdateSpiderPos(elapsedTime);
	}
	else
	{
		UpdatePos(elapsedTime, blocks);
	}

	UpdateTexture(elapsedTime);
	UpdateHands();
}

void Enemy::UpdateRockActivityStatus(Hero const& player)
{
	moveSpeed = ROCK_MOVE_SPEED;
	runStatus = currentRunStatus;

	this->activityStatus = EnemyActivity::IDLE;

	const sf::Vector2f RIGHT_AREA_POS = GetCharacterPos() + ROCK_TARGET_AREA_MARGIN;
	const sf::Vector2f LEFT_MARGIN = sf::Vector2f(-ROCK_TARGET_AREA_SIZE.x, 0);
	const sf::Vector2f LEFT_AREA_POS = GetCharacterPos() + ROCK_TARGET_AREA_SIZE + LEFT_MARGIN;

	if (orientationStatus == OrientationStatus::RIGHT)
	{
		targetArea = sf::FloatRect(RIGHT_AREA_POS, ROCK_TARGET_AREA_SIZE);
	}
	else
	{
		targetArea = sf::FloatRect(LEFT_AREA_POS, ROCK_TARGET_AREA_SIZE);
	}

	if (targetArea.intersects(player.collisionRect))
	{
		moveSpeed = ROCK_PURSUITE_MOVE_SPEED;
		activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::UpdateDragonActivityStatus(Hero const& player)
{
	activityStatus = EnemyActivity::IDLE;

	const float TARGET_AREA_POS_X = GetCharacterPos().x - DRAGON_TARGET_RANGE;
	const float TARGET_AREA_POS_Y = GetCharacterPos().y - 1.5f * DRAGON_TARGET_AREA_SIZE.y;
	sf::Vector2f AREA_POSITION(TARGET_AREA_POS_X, TARGET_AREA_POS_Y);

	targetArea = sf::FloatRect(AREA_POSITION, DRAGON_TARGET_AREA_SIZE);

	if (player.collisionRect.intersects(targetArea))
	{
		activityStatus = EnemyActivity::PURSUIT;
		if (player.GetCharacterPos().x < GetCharacterPos().x)
		{
			orientationStatus = OrientationStatus::LEFT;
		}
		else
		{
			orientationStatus = OrientationStatus::RIGHT;
		}
	}
}

void Enemy::UpdateBirdActivityStatus(Hero const& player)
{
	activityStatus = EnemyActivity::IDLE;

	sf::Vector2f playerPos = player.GetCharacterPos();

	float rangeX = playerPos.x - collisionRect.left;
	float rangeY = playerPos.y - collisionRect.top;

	float targetRadius = sqrt(pow(rangeX, 2) + pow(rangeY, 2));

	if (targetRadius <= BIRD_TARGET_RANGE)
	{
		activityStatus = EnemyActivity::PURSUIT;
	}
	else
	{
		activityStatus = EnemyActivity::IDLE;
	}
}


void Enemy::UpdateSpiderActivityStatus(Hero const& player)
{
	const sf::Vector2f AREA_MARGIN_X(-SPIDER_TARGET_AREA_SIZE.x / 2.0f, 0);
	targetArea = sf::FloatRect(GetCharacterPos() + AREA_MARGIN_X, SPIDER_TARGET_AREA_SIZE);

	if (targetArea.intersects(player.collisionRect))
	{
		activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::UpdateSpiderPos(float elapsedTime)
{
	(void)elapsedTime;
	bodyShape.setPosition(GetCharacterPos());
}

void Enemy::SpiderPursuite(Hero const& player, vector<TmxObject> const& blocks)
{
	auto targetPosX = player.GetCharacterPos().x;

	if (targetPosX < GetCharacterPos().x)
	{
		runStatus = MovementStatus::RUN_LEFT;

		if (!IsCollidesWithLevel(handLeftBottom, blocks) ||
			IsCollidesWithLevel(handLeftMiddle, blocks))
		{
			Jump();
		}
	}
	else if (targetPosX > GetCharacterPos().x)
	{
		runStatus = MovementStatus::RUN_RIGHT;

		if (!IsCollidesWithLevel(handRightBottom, blocks) ||
			IsCollidesWithLevel(handRightMiddle, blocks))
		{
			Jump();
		}
	}
}

void Enemy::RockWalk(float elapsedTime, vector<TmxObject> const& blocks)
{
	if (jumpStatus != JumpingStatus::JUMP)
	{
		sf::FloatRect handLeftMiddle_copy = handLeftMiddle;
		sf::FloatRect handLeftBottom_copy = handLeftBottom;

		sf::FloatRect handRightMiddle_copy = handRightMiddle;
		sf::FloatRect handRightBottom_copy = handRightBottom;

		auto movment = elapsedTime * moveSpeed;

		if (runStatus == MovementStatus::RUN_LEFT)
		{
			handLeftMiddle_copy.left -= movment;
			handLeftBottom_copy.left -= movment;

			if (IsCollidesWithLevel(handLeftMiddle_copy, blocks) ||
				!IsCollidesWithLevel(handLeftBottom_copy, blocks))
			{
				currentRunStatus = MovementStatus::RUN_RIGHT;
			}
		}
		else if (runStatus == MovementStatus::RUN_RIGHT)
		{
			handRightMiddle_copy.left += movment;
			handRightBottom_copy.left += movment;

			if (IsCollidesWithLevel(handRightMiddle_copy, blocks) ||
				!IsCollidesWithLevel(handRightBottom_copy, blocks))
			{
				currentRunStatus = MovementStatus::RUN_LEFT;
			}
		}
	}
}

void Enemy::DragonShoot(vector<Bullet*>& bullets)
{
	int orientationId = static_cast<int>(orientationStatus);

	if (shootColdown >= DRAGON_SHOOT_COLDOWN)
	{
		auto bullet = new Bullet(GetCharacterPos(), shootDamage, orientationId, shootRange, BulletType::DRAGON_BULLET);
		bullets.push_back(bullet);
		shootColdown = 0;
	}
}

void Enemy::UpdateBirdPos(float elapsedTime)
{
	if (ghostMove.x < 0)
	{
		runStatus = MovementStatus::RUN_LEFT;
	}
	else if (ghostMove.x > 0)
	{
		runStatus = MovementStatus::RUN_RIGHT;
	}

	collisionRect.left += elapsedTime * moveSpeed * ghostMove.x;
	collisionRect.top += elapsedTime * moveSpeed * ghostMove.y;

	bodyShape.setPosition(GetCharacterPos());

	ghostMove = { 0 , 0 };
}

void Enemy::BirdIdle(float elapsedTime)
{
	(void)elapsedTime;
}

void Enemy::BirdPursuite(Hero const& player)
{
	sf::Vector2f playerPos = player.GetCharacterPos();

	float halfPlayerBody = player.bodyShape.getSize().y / 2.0f;

	if (playerPos.x < GetCharacterPos().x)
	{
		ghostMove.x = -1;
	}
	else if (playerPos.x > GetCharacterPos().x)
	{
		ghostMove.x = 1;
	}

	if (playerPos.y - halfPlayerBody < GetCharacterPos().y)
	{
		ghostMove.y = -1;
	}
	else if (playerPos.y - halfPlayerBody > GetCharacterPos().y)
	{
		ghostMove.y = 1;
	}
}



void Enemy::UpdateHands()
{
	const float LEFT_HAND_POS_X = GetCharacterPos().x - bodyShape.getSize().x / 2.0f;
	const float RIGHT_HAND_POS_X = GetCharacterPos().x + bodyShape.getSize().x / 2.0f;
	const float BOTTOM_HAND_POS_Y = GetCharacterPos().y + BOTTOM_HAND_MARGIN;
	const float MIDDLE_HAND_POS_Y = GetCharacterPos().y - bodyShape.getSize().y / 4.0f;

	handLeftMiddle = sf::FloatRect({ LEFT_HAND_POS_X, MIDDLE_HAND_POS_Y }, HAND_SIZE);
	handLeftBottom = sf::FloatRect({ LEFT_HAND_POS_X, BOTTOM_HAND_POS_Y }, HAND_SIZE);

	handRightMiddle = sf::FloatRect({ RIGHT_HAND_POS_X, MIDDLE_HAND_POS_Y }, HAND_SIZE);
	handRightBottom = sf::FloatRect({ RIGHT_HAND_POS_X, BOTTOM_HAND_POS_Y }, HAND_SIZE);
}

void Enemy::UpdateTexture(float elapsedTime)
{
	int frameWidth = static_cast<int>(bodyShape.getSize().x);
	int frameHeight = static_cast<int>(bodyShape.getSize().y);
	sf::Vector2i frameSize(frameWidth, frameHeight);
	bodyShape.setFillColor(sf::Color(255, 255, 255, 255));

	int frameColl = 0;
	int frameRow = 0;

	animateTime += elapsedTime;
	int frame = static_cast<int>(animateTime / TIME_TO_FRAME);

	if (orientationStatus == OrientationStatus::LEFT)
	{
		frameColl++;
	}

	if (activityStatus == EnemyActivity::PURSUIT)
	{
		bodyShape.setFillColor(sf::Color(255, 170, 170, 255));
		frameRow++;
	}

	if (frame == FRAMES_COUNT - 1)
	{
		animateTime = 0;
	}

	bodyShape.setTextureRect(
		sf::IntRect({
		(frameColl * FRAMES_COUNT + frame) * frameWidth,
		frameRow * frameHeight }, frameSize)
		);
}