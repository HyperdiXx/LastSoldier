#pragma once
#include "libra.h"

const sf::Vector2f HEALTH_BAR_SIZE = { 180, 70 };
const sf::Vector2f WEAPON_BAR_SIZE = { 110, 80 };
const sf::Vector2f PREVIEW_IMAGE_SIZE = { 1366, 768};//1366, 768
const sf::Vector2f BOX_SIZE = { 58, 50 };
const sf::Vector2f BOSS_BAR_SIZE = { 700, 82 };
const sf::Vector2f BOSS_HP_LINE_SIZE = { 630, 30 };
const sf::Vector2f REMARK_CLOUD_SIZE = { 165, 54 };

const int PLAYER_HP_FONT_SIZE = 50;
const int PLAYER_AMMO_FONT_SIZE = 28;
const int PREVIEW_FONT_SIZE = 35;
const int HELP_FONT_SIZE = 15;
const int ANNOUNCEMENT_FONT_SIZE = 20;
const int REMARK_FONT_SIZE = 16;

const float ANNOUNCEMENT_OUTLINE_THICKNESS = 2;

const sf::Vector2f PLAYER_BARS_MARGIN = { 35, 40 };
const sf::Vector2f PLAYER_HP_MARGIN = { 80, 2 };
const sf::Vector2f PLAYER_WEAPON_MARGIN = { 0, HEALTH_BAR_SIZE.y };
const sf::Vector2f PLAYER_AMMO_MARGIN = { 88, 121 };
const sf::Vector2f PREVIEW_TEXT_MARGIN = { 0, 220 };
const sf::Vector2f PLAYER_BOXES_MARGIN = { 0, HEALTH_BAR_SIZE.y + WEAPON_BAR_SIZE.y + 10 };
const sf::Vector2f BOSS_BAR_MARGIN = { 0, -280 };
const sf::Vector2f BOSS_HP_LINE_MARGIN = { -BOSS_HP_LINE_SIZE.x / 2.0f + 33, -33 };
const sf::Vector2f HELP_TEXT_MARGIN = { 30 , -40 };
const float BOXES_MIDLE_MARGIN = 60;
const int MAX_ANNOUNCEMENT_MARGIN = 40;
const float REMARK_CLOUD_VERTICAL_ORIGIN = 18;
const sf::Vector2f REMARK_CLOUD_MARGIN = { 45, -90 };
const sf::Vector2f REMARK_TEXT_MARGIN = { 0, -2 };

const int PART_COUNT = 3;

const float TRANS_EXTINGUISH_DURATION = 0.5f;
const float PART_DURATION = 3.45f;
const float ANNOUNCEMENT_DURATION = 1;
const float REMARK_DURATION = 2.5f;
const float REMARK_RANDOM_COLDOWN = 25.0f;

const sf::Color BOSS_HP_LINE_COLOR = sf::Color(98, 10, 10, 255);
const sf::Color BOSS_HP_BAR_COLOR = sf::Color(255, 255, 255, 240);
const sf::Color NOT_FOUND_BOX_COLOR = sf::Color(255, 255, 255, 40);

const float DEMAGE_ANNOUNCEMENT_SPEED = 30;

const int PROBABILITY_SIZE = 100; // max order
const float REMARK_KILL_PROBABILITY = 0.2f;
const float REMARK_DEATH_PROBABILITY = 1.0f;
const float REMARK_BONUS_PROBABILITY = 0.4f;
const float REMARK_GIFT_PROBABILITY = 0.5f;
const float REMARK_RANDOM_PROBABILITY = 0.3f;

const std::vector<std::string> KILL_REMARKS = {
	"Easy Breezy",
	"Next please",
	"Take that!",
	"How you like me now?",
	"Now you're done!"
};
const std::vector<std::string> DEATH_REMARKS = {
	"Ohhh, noooo",
	"Lucky u are",
	"God dammit!",
	"I LL be BACK!"
};
const std::vector<std::string> BONUS_REMARKS = {
	"That can be handy",
	"Nice!",
	"That's much better",
	"Right in time!"
};
const std::vector<std::string> GIFT_REMARKS = {
	"Nice!",
	"It`s mine",
	"I've grown up!",
	"I'm like Flash!"
};
const std::vector<std::string> RANDOM_REMARKS = {
	"Forward!",
	"The time has come...",
	"Debbie does Dallas!"
};

enum struct PreviewStatus
{
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	CONTROLLER,
};

enum struct RemarkType
{
	KILL,
	DEATH,
	BONUS,
	GIFT,
	RANDOM,
};

enum struct BackgroundType
{
	BASE,
	CITY,
};

class Cloud
{
public:
	Cloud(std::string const& text);
	~Cloud() {};
	sf::Font font;
	std::string remark;
	sf::Text remarkText;
	sf::RectangleShape cloud;
	sf::Texture cloudTexture;
	float duration = 0;

	void Update(sf::Vector2f const& position, float elapsedTime);
	void Draw(sf::RenderWindow& window);
};

class Effects
{
public:
	Effects::Effects(float width, float height);

	sf::Vector2f resolution;

	sf::Font font;

	sf::RectangleShape playerHealthBar;
	sf::Texture playerHealthBarTexture;
	sf::Text playerHealth;

	Cloud* cloud = nullptr;
	std::map<std::vector<std::string>, float> remarkPacks = {
		{ KILL_REMARKS, REMARK_KILL_PROBABILITY },
		{ DEATH_REMARKS, REMARK_DEATH_PROBABILITY },
		{ BONUS_REMARKS, REMARK_BONUS_PROBABILITY },
		{ GIFT_REMARKS, REMARK_GIFT_PROBABILITY },
		{ RANDOM_REMARKS, REMARK_RANDOM_PROBABILITY }
	};
	std::map<RemarkType, std::map<std::vector<std::string>, float>::iterator> remarkSystem = {
		{ RemarkType::KILL, remarkPacks.find(KILL_REMARKS) },
		{ RemarkType::DEATH, remarkPacks.find(DEATH_REMARKS) },
		{ RemarkType::BONUS, remarkPacks.find(BONUS_REMARKS) },
		{ RemarkType::GIFT, remarkPacks.find(GIFT_REMARKS) },
		{ RemarkType::RANDOM, remarkPacks.find(RANDOM_REMARKS) }
	};
	float randomRemarkColdown = 0;

	sf::RectangleShape currentBackground;
	sf::RectangleShape bg_level_0;
	sf::Texture bgTexture_level_0;
	sf::RectangleShape bg_level_1;
	sf::Texture bgTexture_level_1;

	sf::RectangleShape playerWeaponBar;
	sf::Texture playerMeleeBarTexture;
	sf::Texture playerShootgunBarTexture;
	sf::Texture playerAkBarTexture;
	sf::Text playerWeapon;
	sf::Text playerAmmo;

	std::vector<sf::RectangleShape> filtres;
	sf::RectangleShape blackFilter;
	sf::Texture blackFilterTexture;
	sf::RectangleShape redFilter;
	sf::Texture redFilterTexture;

	sf::RectangleShape previewImage;
	sf::Texture previewTextures[5];
	sf::Text previewText;
	PreviewStatus currentPreviewScene;
	sf::Text helpText;

	std::vector<sf::RectangleShape*> boxes;
	sf::Texture boxTexture;

	sf::RectangleShape bossHPLine;
	sf::RectangleShape bossBar;
	sf::Texture bossBarTexture;

	sf::RectangleShape win;
	sf::Texture winTexture;
	sf::Text winText;

	sf::RectangleShape gameOver;
	sf::Texture gameOverTexture;

	std::vector<sf::Text> damageAnnouncementText;
	std::vector<float> damageAnnouncementDuration;

	float partDuration = 0;
	PreviewStatus currentPart = PreviewStatus::FIRST;

	void CreateAnnouncement(sf::Vector2f const& position, std::string const& str);
	void CreateBoxes(std::map<Tmxlevel*, int> const& boxesMap, Tmxlevel* level);
	void CreateRemark(RemarkType const& type);

	void SetBackground(BackgroundType type);


	void UpdateBackground(sf::Vector2f const& mapSize, sf::Vector2f const& center);
	void UpdateRemark(float elapsedTime, sf::Vector2f const& position);
	void UpdateBarsPos(sf::Vector2f const& cameraPos);
	void UpdatePlayerHP(int health);
	void UpdatePlayerWeapon(int weapon, int ammo = 0);
	void UpdatePlayerBoxes(int currentBoxes);
	void UpdateWin(sf::Vector2f const& windowCenter);
	bool UpdatePreview(sf::Vector2f const& position, float elapsedTime);
	void UpdateHelpButton(std::string const& helpStr, sf::Vector2f const& cameraPos);
	void UpdateAnnouncement(float elapsedTime);

	void Draw(sf::RenderWindow& window);
	void DrawPart(sf::RenderWindow& window);
	void DrawWin(sf::RenderWindow& window);
	void DrawAnnouncement(sf::RenderWindow& window);
};

std::string IntToStr(int number);
sf::Vector2f GetTextureSize(sf::Texture const& texture);
sf::Color Extinguish(
	sf::Color const& color,
	float elementDuration,
	float maxDuration,
	float extinguishDuration = TRANS_EXTINGUISH_DURATION);
	