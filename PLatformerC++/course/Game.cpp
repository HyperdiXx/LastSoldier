#include "Game.h"
#include "mainmenu.h"

using namespace std;
using namespace sf;


enum
{
	// Start menu buttons
	START_MENU_START = 0,
	START_MENU_ABOUT,
	START_MENU_CLOSE,
	// Options menu buttons
	OPTIONS_MENU_EASY = 0,
	OPTIONS_MENU_NORMAL,
	OPTIONS_MENU_HARD,
	OPTIONS_MENU_BACK,
	// Pause menu buttons
	PAUSE_MENU_RESUME = 0,
	PAUSE_MENU_RESTART,
	PAUSE_MENU_EXIT,
};

template <class TContainer, class TPredicate>
void erase_if(TContainer &container, TPredicate && predicate)
{
	auto newEnd = remove_if(begin(container), end(container), [&](auto &pBullet) {
		return predicate(pBullet);
	});
	container.erase(newEnd, container.end());
}

Game::Game(float width, float height)
	:resolution(width, height)
	, visualEffects(width, height)
	, menu(width, height)
	, audio()
{
	srand(static_cast<unsigned>(time(NULL)));

	level_0.LoadFromFile("files/map12.tmx");
	level_1.LoadFromFile("files/map13.tmx");
	level_2.LoadFromFile("files/map14.tmx");

	player.InitPlayer();

	camera.reset(sf::FloatRect({ 0, 0 }, CAMERA_AREA_SIZE));

	difficult = Difficult::EASY;
	menu.Select(MenuType::ABOUT, difficult);
	menu.SetMenu(MenuType::START, camera.getCenter());

	changeLevelMap =
	{
		{ &level_0, &level_1 },
		{ &level_1, &level_2 },
		{ &level_2, &level_0 }
	};

	boxesCountMap =
	{
		{ &level_0, LEVEL_0_BOXES_COUNT },
		{ &level_1, LEVEL_1_BOXES_COUNT },
		{ &level_2, LEVEL_2_BOXES_COUNT }
	};

	sceneMusicMap =
	{
		{ &menuScene, &audio.menuSound },
		{ &gameplayScene, nullptr },
		{ &pauseScene, &audio.menuSound },
		{ &gameOverScene, &audio.gameOverSound },
		{ &previewScene, &audio.menuSound },
		{ &winScene, &audio.winSound }
	};

	levelMusicMap = 
	{
		{ &level_0, &audio.level_0_ost },
		{ &level_1, &audio.level_1_ost },
		{ &level_2, &audio.level_2_ost }
	};

	levelBackgroundMap = 
	{
		{ &level_0, BackgroundType::BASE },
		{ &level_1, BackgroundType::CITY },
		{ &level_2, BackgroundType::CITY }
	};
}

void Game::StartGame()
{
	currentLevel = &level_0;

	ClearScene();
	MapDataAdjustment();

	player.InitPlayer();
	DifficultAdjustment();
	SpawnEntities();

	currentScene = &gameplayScene;
}

void Game::Restart()
{
	ClearScene();

	MapDataAdjustment();

	player.ReturnCopy();
	SpawnEntities();

	currentScene = &gameplayScene;
}

void Game::NextLevel(Tmxlevel& level)
{
	ClearScene();
	player.Clear();
	player.CreateCopy();

	currentLevel = &level;
	MapDataAdjustment();

	SpawnEntities();
}

void Game::DifficultAdjustment()
{
	switch (difficult)
	{
	case Difficult::EASY:
	{
		bonusProbability = EASY_BONUS_PROBABILITY;
		damageIncrease = EASY_DAMAGE_INCREASE;
		break;
	}

	case Difficult::NORMAL:
	{
		bonusProbability = NORMAL_BONUS_PROBABILITY;
		damageIncrease = NORMAL_DAMAGE_INCREASE;
		break;
	}

	case Difficult::HARD:
	{
		bonusProbability = HARD_BONUS_PROBABILITY;
		damageIncrease = HARD_DAMAGE_INCREASE;
		break;
	}
	default:
		break;
	}
}

void Game::MapDataAdjustment()
{
	visualEffects.SetBackground(levelBackgroundMap.find(currentLevel)->second);
	mapSize = currentLevel->GetTilemapSize();
	visualEffects.CreateBoxes(boxesCountMap, currentLevel);
	blocks = currentLevel->GetAllObjects("solid");
	lava = currentLevel->GetAllObjects("lava");
}

void Game::CheckCompletedLevel()
{
	const sf::FloatRect PLAYER_BODY = player.collisionRect;
	const sf::FloatRect LEVEL_DOOR_RECT = currentLevel->GetFirstObject("next_level").rect;

	const int NECESSARY_BOXES_COUNT = boxesCountMap.find(currentLevel)->second;
	const int PLAYER_BOXES_COUNT = player.boxes;

	Tmxlevel* NEXT_LEVEL = changeLevelMap.find(currentLevel)->second;
 	Tmxlevel* START_LEVEL = &level_0;

	if (PLAYER_BODY.intersects(LEVEL_DOOR_RECT) && PLAYER_BOXES_COUNT >= NECESSARY_BOXES_COUNT)
	{
		if (NEXT_LEVEL == START_LEVEL)
		{
			currentScene = &winScene;
		}

		NextLevel(*NEXT_LEVEL);
	}
}

void Game::ClearScene()
{
	enemies.clear();
	bonuses.clear();
	enemyBullets.clear();
	delete visualEffects.cloud;
	visualEffects.damageAnnouncementText.clear();
	visualEffects.damageAnnouncementDuration.clear();

	visualEffects.cloud = nullptr;
}

void Game::SpawnEntities()
{
	const vector<TmxObject> ROCK_SPAWNS = currentLevel->GetAllObjects("enemy_rock_spawn");
	const vector<TmxObject> DRAGON_SPAWNS = currentLevel->GetAllObjects("enemy_dragon_spawn");
	const vector<TmxObject> GHOST_SPAWNS = currentLevel->GetAllObjects("enemy_bird_spawn");
	const vector<TmxObject> SPIDER_SPAWNS = currentLevel->GetAllObjects("enemy_spider_spawn");
	

	const vector<TmxObject> BOX_SPAWNS = currentLevel->GetAllObjects("item_box_spawn");
	const vector<TmxObject> HEALTH_SPAWNS = currentLevel->GetAllObjects("bonus_heath");
	const vector<TmxObject> AK_AMMO_SPAWNS = currentLevel->GetAllObjects("bonus_ak_ammo");
	const vector<TmxObject> GIFT_SPAWNS = currentLevel->GetAllObjects("bonus_random");
	const vector<TmxObject> SHOOTGUN_AMMO_SPAWNS = currentLevel->GetAllObjects("bonus_shootgun_ammo");

	SpawnEnemies(ROCK_SPAWNS, EnemyType::ROCK);
	SpawnEnemies(DRAGON_SPAWNS, EnemyType::DRAGON);
	SpawnEnemies(GHOST_SPAWNS, EnemyType::BIRD);
	SpawnEnemies(SPIDER_SPAWNS, EnemyType::SPIDER);

	SpawnItems(BOX_SPAWNS, BonusType::ITEM_BOX);
	SpawnItems(HEALTH_SPAWNS, BonusType::HEALTH);
	SpawnItems(AK_AMMO_SPAWNS, BonusType::AK_AMMO);
	SpawnItems(GIFT_SPAWNS, BonusType::GIFT);
	SpawnItems(SHOOTGUN_AMMO_SPAWNS, BonusType::SHOOTGUN_AMMO);

	const sf::Vector2f PLAYER_POS = currentLevel->GetFirstObject("player_spawn").sprite.getPosition();
	player.Spawn(PLAYER_POS);
}

void Game::SpawnItems(vector<TmxObject> const& spawns, BonusType const& type)
{
	for (auto const& spawn : spawns)
	{
		sf::Vector2f pos = spawn.sprite.getPosition();
		bonuses.push_back(new Bonus(pos, type));
	}
}

void Game::SpawnEnemies(vector<TmxObject> const& spawns, EnemyType const& type)
{
	for (auto const& spawn : spawns)
	{
		sf::Vector2f pos = spawn.sprite.getPosition();
		enemies.push_back(new Enemy(pos, type, damageIncrease));
	}
}

void Game::SetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

sf::FloatRect Game::GetCameraArea()
{
	auto camCenter = camera.getCenter();
	sf::FloatRect cameraArea(
		camCenter.x - resolution.x / 2.0f,
		camCenter.y - resolution.y / 2.0f, resolution.x, resolution.y
		);

	return cameraArea;
}

bool Game::IsCollidesWithLevel(sf::FloatRect const& rect)
{
	return any_of(blocks.begin(), blocks.end(), [&](const TmxObject&block) {
		return (rect.intersects(block.rect) && block.name == "solid");
	});
}

void Game::ControlPlayer()
{
	
	
	if (Keyboard::isKeyPressed(Keyboard::Escape) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.SetMenu(MenuType::PAUSE, camera.getCenter());
		currentScene = &menuScene;
	}
	else if (player.existStatus != ExistenceStatus::DEAD)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			player.Jump();
		}
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.runStatus = RUN_LEFT;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.runStatus = RUN_RIGHT;
		}
		if (Keyboard::isKeyPressed(Keyboard::Q) && player.switchWeaponColdown >= BUTTONS_COLDOWN)
		{
			player.switchWeaponColdown = 0;
			player.SwitchWeapon();
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			player.Attack();
		}
	}
}

void Game::ControlMenu(sf::RenderWindow& window)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape) && menu.buttonsColdown >= BUTTONS_COLDOWN && menu.currentMenu == MenuType::PAUSE)
	{
		menu.buttonsColdown = 0;
		currentScene = &gameplayScene;
	}
	else if ((Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::Return)) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.buttonsColdown = 0;
		ControlMenuLogic(window);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.SwitchButtonUp();
		audio.menuButtonSwitchSound.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.SwitchButtonDown();
		audio.menuButtonSwitchSound.play();
	}
}

void Game::ControlMenuLogic(sf::RenderWindow& window)
{
	switch (menu.currentMenu)
	{
	case MenuType::START:
		switch (menu.currentButton)
		{
		case START_MENU_START:
			StartGame();
			break;
		case START_MENU_ABOUT:
			menu.SetMenu(MenuType::ABOUT, camera.getCenter());
			break;
		case START_MENU_CLOSE:
			window.close();
			break;
		default:
			break;
		}
		menu.currentButton = 0;
		break;
	case MenuType::ABOUT:
		switch (menu.currentButton)
		{
		case OPTIONS_MENU_EASY:
			difficult = Difficult::EASY;
			break;
		case OPTIONS_MENU_NORMAL:
			difficult = Difficult::NORMAL;
			break;
		case OPTIONS_MENU_HARD:
			difficult = Difficult::HARD;
			break;
		case OPTIONS_MENU_BACK:
			menu.SetMenu(MenuType::START, camera.getCenter());
			break;
		default:
			break;
		}
		menu.Select(MenuType::ABOUT, difficult);
		break;

	case MenuType::PAUSE:
		switch (menu.currentButton)
		{
		case PAUSE_MENU_RESUME:
			currentScene = &gameplayScene;
			break;
		case PAUSE_MENU_RESTART:
			Restart();
			break;
		case PAUSE_MENU_EXIT:
			menu.SetMenu(MenuType::START, camera.getCenter());
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Game::ControlGameOver(sf::RenderWindow& window)
{
	(void)window;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		Restart();
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Escape))
	{
		menu.SetMenu(MenuType::START, camera.getCenter());
		currentScene = &menuScene;
	}
}

void Game::UpdatePlayer()
{
	player.UpdateTexture(elapsedTime);
	player.UpdateOrientation();
	player.UpdatePos(elapsedTime, blocks);
	player.UpdateHealthStatus();
	player.UpdateStatuses();

	if (player.existStatus == ExistenceStatus::DEAD)
	{
		visualEffects.CreateRemark(RemarkType::DEATH);
		player.RotateDeadBody(elapsedTime);

		if (gameOverColdown >= GAME_OVER_DURATION)
		{
			gameOverColdown = 0;
			currentScene = &gameOverScene;
		}
	}

	CheckCompletedLevel();
}

void Game::UpdateBullets()
{
	auto updateBullets = [&](vector<Bullet*> &bullets) {
		for (Bullet* pBullet : bullets)
		{
			pBullet->Update(elapsedTime);
		}
		erase_if(bullets, [&](Bullet* pBullet) {
			bool dead = (IsCollidesWithLevel(pBullet->collisionRect) || !pBullet->isLive);
			if (dead)
				delete(pBullet);

			return dead;
		});
	};

	updateBullets(player.characterBullets);
	updateBullets(enemyBullets);
}

void Game::UpdateEnemies()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* enemy = *it;
		enemy->UpdateAI(elapsedTime, player, blocks, enemyBullets);

		if (enemy->existStatus == ExistenceStatus::DEAD)
		{
			audio.EnemyDeath(static_cast<int>(enemy->enemyType));
			explosions.push_back(new Explosion(enemy->GetCharacterPos()));
			visualEffects.CreateRemark(RemarkType::KILL);
			/*if (enemy->enemyType == EnemyType::BOSS)
			{
				bonuses.push_back(new Bonus(enemy->GetCharacterPos(), BonusType::ITEM_BOX));
			}
			else
			{
				DropBonusFromEnemy(enemy->GetCharacterPos(), bonuses, bonusProbability);
			}*/
			it = enemies.erase(it);
			delete(enemy);
		}
		else
		{
			++it;
		}
	}
}

void Game::UpdateTemporaryObjects()
{
	for (auto enemy : enemies)
	{
		for (auto blood : enemy->wounds)
		{
			blood->Update(enemy->GetCharacterPos(), elapsedTime);
		}
		erase_if(enemy->wounds, [&](Blood* blood) {
			bool durationEnd = blood->duration >= BLOOD_DURATION;
			if (durationEnd)
				delete(blood);

			return durationEnd;
		});
	}

	for (auto blood : player.wounds)
	{
		blood->Update(player.GetCharacterPos(), elapsedTime);
	}
	erase_if(player.wounds, [&](Blood* blood) {
		bool durationEnd = blood->duration >= BLOOD_DURATION;
		if (durationEnd)
			delete(blood);

		return durationEnd;
	});

	for (auto fire : player.gunFire)
	{
		fire->Update(player.GetCharacterPos(), elapsedTime, player.orientationStatus);
	}
	erase_if(player.gunFire, [&](GunFire* fire) {
		bool durationEnd = fire->duration >= GUNFIRE_DURATION;
		if (durationEnd)
			delete(fire);

		return durationEnd;
	});

	for (auto explosion : explosions)
	{
		explosion->Update(elapsedTime);
	}
	erase_if(explosions, [&](Explosion* explosion) {
		bool durationEnd = explosion->duration >= EXPLOSION_DURATION;
		if (durationEnd)
			delete(explosion);

		return durationEnd;
	});
}

void Game::UpdateBonuses()
{
	for (auto bonus : bonuses)
	{
		bonus->Update(elapsedTime, blocks);
	}
}

void Game::UpdateOST()
{
	sf::Music* OST;

	if (currentScene == &gameplayScene)
	{
		OST = levelMusicMap.find(currentLevel)->second;
	}
	else
	{
		OST = sceneMusicMap.find(currentScene)->second;
	}

	if (OST != audio.currentMusic)
	{
		if (audio.currentMusic != nullptr)
		{
			audio.currentMusic->stop();
		}

		audio.currentMusic = OST;
	}

	if (audio.currentMusic->getStatus() != sf::Music::Playing)
	{
		audio.currentMusic->play();
	}
}

void Game::CheckEntitiesCollides()
{
	EnemyPlayerCollides();
	PlayerBulletsEnemyCollides();
	EnemyBulletsPlayerCollides();
	BonusesPlayerCollides();
	PlayerLavaCollides();
}

void Game::EnemyBulletsPlayerCollides()
{
	for (auto bullet : enemyBullets)
	{
		if (player.collisionRect.intersects(bullet->collisionRect))
		{
			if (player.injuredColdown >= INJURED_DURATION)
			{
				PlayWithoutDouble(player.playerHurtGrunt);
				player.health -= bullet->damage;
				player.wounds.push_back(new Blood(player.GetCharacterPos(), bullet->bodyShape.getPosition()));
				bullet->isLive = false;
			}
		}
	}
}

void Game::PlayerLavaCollides()
{
	for (auto lavaBlock : this->lava)
	{
		if (lavaBlock.rect.intersects(player.collisionRect))
		{
			if (player.injuredColdown >= INJURED_DURATION)
			{
				PlayWithoutDouble(player.playerHurtGrunt);
				player.health -= LAVA_DEMAGE;
				player.injuredColdown = 0;
			}
		}
	}
}

void Game::BonusesPlayerCollides()
{
	for (auto it = bonuses.begin(); it != bonuses.end();)
	{
		Bonus* bonus = *it;
		if (bonus->collisionRect.intersects(player.collisionRect) && bonus->AddBonusEffect(player))
		{
			if (bonus->bonusType == BonusType::GIFT)
				visualEffects.CreateRemark(RemarkType::GIFT);
			else
				visualEffects.CreateRemark(RemarkType::BONUS);

			const sf::Vector2f BONUS_POSITION(bonus->bodyShape.getPosition());
			visualEffects.CreateAnnouncement(BONUS_POSITION, bonus->announcementText);
			audio.CollideWithBonus(static_cast<int>(bonus->bonusType));

			it = bonuses.erase(it);
			delete(bonus);
		}
		else
		{
			++it;
		}
	}
}

void Game::PlayerBulletsEnemyCollides()
{
	for (auto bullet : player.characterBullets)
	{
		for (auto enemy : enemies)
		{
			if (enemy->collisionRect.intersects(bullet->collisionRect))
			{
				const string dmgStr = "-" + to_string(bullet->damage);
				enemy->health -= bullet->damage;
				enemy->activityStatus = EnemyActivity::PURSUIT;
				enemy->wounds.push_back(new Blood(enemy->GetCharacterPos(), bullet->bodyShape.getPosition()));
				visualEffects.CreateAnnouncement(bullet->bodyShape.getPosition(), dmgStr);
				bullet->isLive = false;
			}
		}
	}
}

void Game::EnemyPlayerCollides()
{
	for (auto enemy : enemies)
	{
		if (enemy->collisionRect.intersects(player.collisionRect))
		{
			if (player.injuredColdown >= INJURED_DURATION)
			{
				PlayWithoutDouble(player.playerHurtGrunt);
				audio.CollideWithEnemy((int)enemy->enemyType);
				player.health -= enemy->touchDamage;
				player.injuredColdown = 0;
			}
		}
	}
}

void Game::UpdateColdowns()
{
	auto AddTime = [&](float& timer, float limit, bool criterion = true) {
		if (timer <= limit && criterion) { timer += elapsedTime; }
	};

	if (currentScene == &gameplayScene)
	{
		AddTime(player.shootColdown, MAX_WEAPON_COLDOWN);
		AddTime(player.injuredColdown, INJURED_DURATION);
		AddTime(player.switchWeaponColdown, BUTTONS_COLDOWN);
		AddTime(visualEffects.randomRemarkColdown, REMARK_RANDOM_COLDOWN);
		AddTime(gameOverColdown, GAME_OVER_DURATION, player.existStatus == ExistenceStatus::DEAD);

		for (auto enemy : enemies)
		{
			AddTime(enemy->shootColdown, DRAGON_SHOOT_COLDOWN, enemy->enemyType == EnemyType::DRAGON);
			//AddTime(enemy->shootColdown, BOSS_SHOOT_COLDOWN, enemy->enemyType == EnemyType::BOSS);
		}
	}

	AddTime(menu.buttonsColdown, BUTTONS_COLDOWN);
}

void Game::UpdateCamera(sf::RenderWindow& window)
{
	sf::Vector2f halfWindow = { CAMERA_AREA_SIZE * 0.5f };
	sf::Vector2f cameraCenter = {
		player.GetCharacterPos().x,
		player.GetCharacterPos().y - CAMERA_VERTICAL_MARGIN
	};
	camera.setCenter(cameraCenter);

	if (cameraCenter.x - halfWindow.x < 0)
	{
		camera.setCenter(halfWindow.x, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.x + halfWindow.x > mapSize.x)
	{
		camera.setCenter(mapSize.x - halfWindow.x, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y - halfWindow.y < 0)
	{
		camera.setCenter(cameraCenter.x, halfWindow.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y + halfWindow.y > mapSize.y)
	{
		camera.setCenter(cameraCenter.x, mapSize.y - halfWindow.y);
		cameraCenter = camera.getCenter();
	}

	window.setView(camera);
}

void Game::UpdateInterface()
{
	const int AMMO = player.ammoMap.find(player.currentWeapon)->second;

	visualEffects.UpdateBackground(currentLevel->GetTilemapSize(), camera.getCenter());
	visualEffects.UpdateBarsPos(camera.getCenter());
	visualEffects.UpdatePlayerHP(player.health);
	visualEffects.UpdatePlayerWeapon(player.currentWeapon, AMMO);
	visualEffects.UpdatePlayerBoxes(player.boxes);
	visualEffects.UpdateAnnouncement(elapsedTime);
	visualEffects.UpdateRemark(elapsedTime, player.GetCharacterPos());

	if (visualEffects.randomRemarkColdown >= REMARK_RANDOM_COLDOWN)
	{
		visualEffects.CreateRemark(RemarkType::RANDOM);
		visualEffects.randomRemarkColdown = 0;
	}

	/*for (auto enemy : enemies)
	{
		if (enemy->enemyType == EnemyType::BOSS)
		{
			visualEffects.UpdateBossBar(BOSS_START_HEALTH, enemy->health);
		}
	}*/
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	currentLevel->Draw(window, GetCameraArea());
}

void Game::DrawBullets(sf::RenderWindow& window) // player bullets generally in window
{
	for (auto const& playerBullet : player.characterBullets)
	{
		window.draw(playerBullet->bodyShape);
	}

	for (auto const& bullet : enemyBullets)
	{
		if (GetCameraArea().intersects(bullet->collisionRect))
		{
			window.draw(bullet->bodyShape);
		}
	}
}

void Game::DrawEnemies(sf::RenderWindow& window)
{
	for (auto const& enemy : enemies)
	{
		if (GetCameraArea().intersects(enemy->collisionRect))
		{
			enemy->DrawCharacter(window);
		}
	}
}

void Game::DrawBonuses(sf::RenderWindow& window)
{
	for (auto const& bonus : bonuses)
	{
		if (GetCameraArea().intersects(bonus->collisionRect))
		{
			bonus->DrawBonus(window);
		}
	}
}

void Game::DrawTemporaryObjects(sf::RenderWindow& window)
{
	for (auto explosion : explosions)
	{
		explosion->Draw(window);
	}
}

void Game::DrawInterface(sf::RenderWindow& window)
{
	visualEffects.Draw(window);

	/*for (auto const& enemy : enemies)
	{
		if (enemy->enemyType == EnemyType::BOSS)
		{
			visualEffects.DrawBossBar(window);
		}
	}*/
}