#pragma once
#include "libra.h"
#include "mainmenu.h"
#include "Player.h"
#include "interface.h"
#include "Enemy.h"
#include "bonus.h"
#include "Sounds.h"
#include "bullets.h"

const float CAMERA_VERTICAL_MARGIN = 80;
const sf::Vector2f CAMERA_AREA_SIZE = { 1280, 720 };
const sf::Color BACKGROUND_COLOR = sf::Color(20, 12, 28);

const int LEVEL_0_BOXES_COUNT = 0;
const int LEVEL_1_BOXES_COUNT = 3;
const int LEVEL_2_BOXES_COUNT = 3;

class GameScene
{
public:
	GameScene() {};
	~GameScene() {};
	std::function<void(sf::RenderWindow& window)> toHandle;
	std::function<void()> onUpdate;
	std::function<void(sf::RenderWindow& window)> onDraw;
};

class Game
{
public:
	Game(float width, float height);
	~Game() {};
	sf::Vector2f resolution;

	sf::View camera;
	Effects visualEffects;
	GameSound audio;
	Menu menu;

	sf::Clock clock;
	float elapsedTime;
	float gameOverColdown = 0;

	std::map<GameScene*, sf::Music*> sceneMusicMap;
	std::map<Tmxlevel*, sf::Music*> levelMusicMap;

	Player player;
	std::vector<Enemy*> enemies;
	std::vector<Bullet*> enemyBullets;
	std::vector<Bonus*> bonuses;
	std::vector<Explosion*> explosions;

	std::vector<TmxObject> blocks;
	std::vector<TmxObject> lava;
	sf::Vector2f mapSize;

	Tmxlevel* currentLevel;
	Tmxlevel level_0;
	Tmxlevel level_1;
	Tmxlevel level_2;
	Tmxlevel level_3;
	std::map<Tmxlevel*, Tmxlevel*> changeLevelMap;
	std::map<Tmxlevel*, int> boxesCountMap;
	std::map<Tmxlevel*, BackgroundType> levelBackgroundMap;

	Difficult difficult;

	GameScene gameplayScene;
	GameScene menuScene;
	GameScene pauseScene;
	GameScene gameOverScene;
	GameScene previewScene;
	GameScene winScene;
	GameScene *currentScene = nullptr;

	int bonusProbability;
	int damageIncrease;

	void StartGame();
	void Restart();
	void NextLevel(Tmxlevel& level);
	void MapDataAdjustment();

	void DifficultAdjustment();

	void CheckCompletedLevel();

	void SpawnEntities();
	void SpawnItems(std::vector<TmxObject> const& spawns, BonusType const& type);
	void SpawnEnemies(std::vector<TmxObject> const& spawns, EnemyType const& type);

	void SetElapsedTime();
	sf::FloatRect GetCameraArea();

	bool IsCollidesWithLevel(sf::FloatRect const& rect);

	void ControlPlayer();
	void ControlMenu(sf::RenderWindow& window);
	void ControlMenuLogic(sf::RenderWindow& window);
	void ControlGameOver(sf::RenderWindow& window);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateColdowns();
	void UpdatePlayer();
	void UpdateBullets();
	void UpdateEnemies();
	void UpdateTemporaryObjects();
	void UpdateInterface();
	void UpdateBonuses();
	void UpdateOST();

	void CheckEntitiesCollides();
	void EnemyPlayerCollides();
	void PlayerBulletsEnemyCollides();
	void EnemyBulletsPlayerCollides();
	void BonusesPlayerCollides();
	void PlayerLavaCollides();

	void DrawLevel(sf::RenderWindow& window);
	void DrawBullets(sf::RenderWindow& window);
	void DrawEnemies(sf::RenderWindow& window);
	void DrawBonuses(sf::RenderWindow& window);
	void DrawTemporaryObjects(sf::RenderWindow& window);
	void DrawInterface(sf::RenderWindow& window);

	void ClearScene();
}; 
