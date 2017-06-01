#include "Interface.h"

using namespace std;
using namespace sf;


enum
{
	// Weapon type
	PISTOL = 0,
	SHOOTGUN = 1,
	AK = 2,
	// Ammo count
	INFINITY_AMMO = INT_MAX,
};

Effects::Effects(float width, float height)
	:resolution(width, height)
{
	font.loadFromFile("files/fo.ttf");
	playerHealthBarTexture.loadFromFile("files/images/HP.png");
	playerMeleeBarTexture.loadFromFile("files/images/PH.png");
	playerShootgunBarTexture.loadFromFile("files/images/SH.png");
	playerAkBarTexture.loadFromFile("files/images/AK.png");
	gameOverTexture.loadFromFile("files/images/died.png");
	blackFilterTexture.loadFromFile("resources/blackFilter.png");
	previewTextures[0].loadFromFile("files/images/startM1.png");
	previewTextures[1].loadFromFile("files/images/startM2.png");
	previewTextures[2].loadFromFile("files/images/startM3.png");
	previewTextures[3].loadFromFile("files/images/startM4.png");
	boxTexture.loadFromFile("files/images/cd.png");
	bossBarTexture.loadFromFile("resources/bossBar.png");
	winTexture.loadFromFile("files/images/victor.png");
	bgTexture_level_0.loadFromFile("files/images/bg1.png");
	bgTexture_level_1.loadFromFile("files/images/bg1.png");

	playerHealth = Text("", font, PLAYER_HP_FONT_SIZE);
	playerAmmo = Text("", font, PLAYER_AMMO_FONT_SIZE);	
	previewText = Text("", font, PREVIEW_FONT_SIZE);
	winText = Text("", font, PREVIEW_FONT_SIZE);
	helpText = Text("", font, HELP_FONT_SIZE);

	playerHealthBar.setSize(GetTextureSize(playerHealthBarTexture));
	playerHealthBar.setTexture(&playerHealthBarTexture);
	playerWeaponBar.setSize(WEAPON_BAR_SIZE);

	bossBar.setSize(BOSS_BAR_SIZE);
	bossBar.setOrigin(BOSS_BAR_SIZE.x / 2.0f, BOSS_BAR_SIZE.y / 2.0f);
	bossBar.setTexture(&bossBarTexture);
	bossBar.setFillColor(BOSS_HP_BAR_COLOR);
	bossHPLine.setSize(BOSS_HP_LINE_SIZE);
	bossHPLine.setFillColor(BOSS_HP_LINE_COLOR);

	gameOver.setTexture(&gameOverTexture);
	gameOver.setSize(GetTextureSize(gameOverTexture));
	gameOver.setOrigin(gameOver.getSize().x / 2.0f, gameOver.getSize().y / 2.0f);

	win.setTexture(&winTexture);
	win.setSize(GetTextureSize(winTexture));
	win.setOrigin(win.getSize().x / 2.0f, win.getSize().y / 2.0f);
	winText.setString("Hope remains");
	winText.setOrigin(winText.getGlobalBounds().width / 2.0f, winText.getGlobalBounds().height / 2.0f);

	blackFilter.setSize(resolution);
	blackFilter.setTexture(&blackFilterTexture);
	blackFilter.setOrigin(resolution.x / 2.0f, resolution.y / 2.0f);

	const sf::Vector2f BG_LVL_0_SISE = GetTextureSize(bgTexture_level_0);
	const sf::Vector2f BG_LVL_1_SISE = GetTextureSize(bgTexture_level_1);

	bg_level_1.setTexture(&bgTexture_level_1);
	bg_level_1.setSize(BG_LVL_1_SISE);
	bg_level_1.setOrigin(BG_LVL_1_SISE.x / 2.0f, BG_LVL_1_SISE.y / 2.0f);

	bg_level_0.setTexture(&bgTexture_level_0);
	bg_level_0.setSize(BG_LVL_0_SISE);
	bg_level_0.setOrigin(BG_LVL_0_SISE.x / 2.0f, BG_LVL_0_SISE.y / 2.0f);

	previewImage.setSize(PREVIEW_IMAGE_SIZE);
	previewImage.setOrigin(PREVIEW_IMAGE_SIZE.x / 2.0f, PREVIEW_IMAGE_SIZE.y / 2.0f);
}

void Effects::CreateBoxes(map<Tmxlevel*, int> const& boxesMap, Tmxlevel* level)
{
	const int maxBoxes = boxesMap.find(level)->second;
	boxes.clear();

	for (int i = 0; i < maxBoxes; i++)
	{
		boxes.push_back(new sf::RectangleShape(BOX_SIZE));
	}

	for (auto box : boxes)
	{
		box->setTexture(&boxTexture);
	}
}

void Effects::CreateAnnouncement(sf::Vector2f const& position, string const& str)
{
	auto marginX = static_cast<float>(rand() % MAX_ANNOUNCEMENT_MARGIN + 1);
	auto marginY = static_cast<float>(rand() % MAX_ANNOUNCEMENT_MARGIN + 1);

	sf::Vector2f margin(marginX, marginY / 2.0f);

	sf::Text announcement = Text(str, font, ANNOUNCEMENT_FONT_SIZE);
	announcement.setOutlineThickness(ANNOUNCEMENT_OUTLINE_THICKNESS);
	announcement.setOutlineColor(sf::Color::Black);
	announcement.setOrigin(announcement.getGlobalBounds().width / 2.0f, announcement.getGlobalBounds().height / 2.0f);
	announcement.setPosition(position + margin);

	damageAnnouncementText.push_back(announcement);
	damageAnnouncementDuration.push_back(0);
}

void Effects::CreateRemark(RemarkType const& type)
{
	if (cloud == nullptr)
	{
		const float probability = remarkSystem.find(type)->second->second;

		if (rand() % PROBABILITY_SIZE < static_cast<int>(probability * PROBABILITY_SIZE))
		{
			const size_t remarksCount = remarkSystem.find(type)->second->first.size();
			const int randomRemarkNumber = rand() % remarksCount;
			const string text = remarkSystem.find(type)->second->first[randomRemarkNumber];
			cloud = new Cloud(text);
		}
	}
}

void Effects::SetBackground(BackgroundType type)
{
	switch (type)
	{
	case BackgroundType::BASE:
		currentBackground = bg_level_0;
		break;
	case BackgroundType::CITY:
		currentBackground = bg_level_1;
		break;
	default:
		break;
	}
}

void Effects::UpdateBackground(sf::Vector2f const& mapSize, sf::Vector2f const& center)
{
	auto backgroundSize = currentBackground.getSize();

	float bgPosX_Percent = (center.x - resolution.x / 2.0f) / (mapSize.x - resolution.x);
	float bgPosY_Percent = (center.y - resolution.y / 2.0f) / (mapSize.y - resolution.y);
	sf::Vector2f bgPos_Percent(bgPosX_Percent, bgPosY_Percent);

	sf::Vector2f bgAllowedSize(mapSize.x - backgroundSize.x, mapSize.y - backgroundSize.y);

	sf::Vector2f bgPos_Pixel = backgroundSize * 0.5f + sf::Vector2f(bgPos_Percent.x * bgAllowedSize.x, bgPos_Percent.y * bgAllowedSize.y);

	currentBackground.setPosition(bgPos_Pixel);
}

void Effects::UpdateRemark(float elapsedTime, sf::Vector2f const& position)
{
	if (cloud != nullptr)
	{
		if (cloud->duration >= REMARK_DURATION)
		{
			delete cloud;
			cloud = nullptr;
		}
		else
		{
			cloud->Update(position, elapsedTime);
		}
	}
}

void Effects::UpdateBarsPos(Vector2f const& cameraPos)
{
	sf::Vector2f playerHealthPos = cameraPos - resolution * 0.5f + PLAYER_BARS_MARGIN;
	sf::Vector2f playerWeaponBarPos = playerHealthPos + PLAYER_WEAPON_MARGIN;

	playerHealthBar.setPosition(playerHealthPos);
	playerWeaponBar.setPosition(playerWeaponBarPos);

	playerHealth.setPosition(playerHealthPos + PLAYER_HP_MARGIN);

	playerAmmo.setOrigin(playerAmmo.getGlobalBounds().width / 2.0f, playerAmmo.getGlobalBounds().height / 2.0f);
	playerAmmo.setPosition(playerHealthPos + PLAYER_AMMO_MARGIN);

	bossBar.setPosition(cameraPos + BOSS_BAR_MARGIN);
	bossHPLine.setPosition(cameraPos + BOSS_BAR_MARGIN + BOSS_HP_LINE_MARGIN);

	blackFilter.setPosition(cameraPos);

	for (auto box : boxes)
	{
		box->setPosition(playerHealthPos + PLAYER_BOXES_MARGIN);
	}
}

void Effects::UpdatePlayerHP(int health)
{
	string hpStr = IntToStr(health);
	playerHealth.setString(hpStr);
}

void Effects::UpdatePlayerWeapon(int weapon, int ammo)
{
	switch (weapon)
	{
	case PISTOL:
		playerWeaponBar.setTexture(&playerMeleeBarTexture);
		break;
	case SHOOTGUN:
		playerWeaponBar.setTexture(&playerShootgunBarTexture);
		break;
	case AK:
		playerWeaponBar.setTexture(&playerAkBarTexture);
		break;
	default:
		break;
	}

	if (ammo >= 0 && ammo != INFINITY_AMMO)
	{
		string ammoStr = IntToStr(ammo);
		playerAmmo.setString(ammoStr);
	}
	else
	{
		playerAmmo.setString("");
	}
}

bool Effects::UpdatePreview(sf::Vector2f const& position, float elapsedTime)
{
	if (partDuration >= PART_DURATION)
	{
		if (currentPart == PreviewStatus::FOURTH)
		{
			return true;
		}

		partDuration = 0;
		currentPart = PreviewStatus(static_cast<int>(currentPart) + 1);
		previewText.setFillColor(sf::Color::White);
		previewImage.setFillColor(sf::Color::White);
	}

	partDuration += elapsedTime;
	previewImage.setPosition({ position.x, position.y });
	previewText.setOrigin(previewText.getGlobalBounds().width / 2.0f, previewText.getGlobalBounds().height / 2.0f);
	previewText.setPosition(position + PREVIEW_TEXT_MARGIN);

	const sf::Color color = previewImage.getFillColor();
	previewImage.setFillColor(Extinguish(color, partDuration, PART_DURATION));
	previewText.setFillColor(Extinguish(color, partDuration, PART_DURATION));

	switch (currentPart)
	{
	case PreviewStatus::FIRST:
		previewImage.setTexture(&previewTextures[0]);
		previewText.setString("They were usual terrofighters in the time...");
		break;
	case PreviewStatus::SECOND:
		previewImage.setTexture(&previewTextures[1]);
		previewText.setString("Defending their county in different places...");
		break;
	case PreviewStatus::THIRD:
		previewImage.setTexture(&previewTextures[2]);
		previewText.setString("One day the world has changed...");
		break;
	case PreviewStatus::FOURTH:
		previewImage.setTexture(&previewTextures[3]);
		previewText.setString("No one could find the answers...");
		break;
	/*case PreviewStatus::FIFTH:
		previewImage.setTexture(&previewTextures[4]);
		previewText.setString("No one could find the answers...");
		break;*/
	default:
		break;
	}

	UpdateHelpButton("Press \"ENTER\" to skip", position);

	return false;
}

void Effects::UpdateHelpButton(string const& helpStr, sf::Vector2f const& cameraPos)
{
	const float HELP_POS_X = cameraPos.x - resolution.x * 0.5f;
	const float HELP_POS_Y = cameraPos.y + resolution.y * 0.5f;
	const sf::Vector2f HELP_POS(HELP_POS_X, HELP_POS_Y);

	helpText.setString(helpStr);
	helpText.setPosition(HELP_POS + HELP_TEXT_MARGIN);
}

void Effects::UpdatePlayerBoxes(int currentBoxes)
{
	if (!boxes.empty())
	{
		float firstPosX = boxes.front()->getPosition().x;
		int boxNumber = 0;

		for (auto box : boxes)
		{
			if (boxNumber < currentBoxes)
			{
				box->setFillColor(sf::Color::White);
			}
			else
			{
				box->setFillColor(NOT_FOUND_BOX_COLOR);
			}
			box->setPosition({ firstPosX + boxNumber * BOXES_MIDLE_MARGIN, box->getPosition().y });
			boxNumber++;
		}
	}
}


void Effects::UpdateWin(sf::Vector2f const& windowCenter)
{
	win.setPosition(windowCenter);
	winText.setPosition(windowCenter + PREVIEW_TEXT_MARGIN);
	UpdateHelpButton("Press \"ENTER\" to go main menu", windowCenter);
}

void Effects::UpdateAnnouncement(float elapsedTime)
{
	for (size_t elementNumber = 0; elementNumber < damageAnnouncementDuration.size();)
	{
		float* duration = &damageAnnouncementDuration[elementNumber];
		sf::Text* text = &damageAnnouncementText[elementNumber];

		if (*duration >= ANNOUNCEMENT_DURATION)
		{
			damageAnnouncementDuration.erase(damageAnnouncementDuration.begin() + elementNumber);
			damageAnnouncementText.erase(damageAnnouncementText.begin() + elementNumber);
		}
		else
		{
			text->move(0, -DEMAGE_ANNOUNCEMENT_SPEED * elapsedTime);

			const sf::Color color = text->getFillColor();
			sf::Color newColor(Extinguish(color, *duration, ANNOUNCEMENT_DURATION));
			text->setFillColor(newColor);
			text->setOutlineColor(sf::Color(0, 0, 0, newColor.a));

			*duration += elapsedTime;
			++elementNumber;
		}
	}
}

void Effects::Draw(RenderWindow& window)
{
	window.draw(blackFilter);
	window.draw(playerHealthBar);
	window.draw(playerHealth);
	window.draw(playerWeaponBar);
	window.draw(playerAmmo);
	DrawAnnouncement(window);

	if (cloud != nullptr)
	{
		cloud->Draw(window);
	}

	for (auto it = boxes.begin(); it != boxes.end(); it++)
	{
		sf::RectangleShape* box = *it;
		window.draw(*box);
	}
}

void Effects::DrawPart(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	window.draw(previewImage);
	window.draw(previewText);
	window.draw(helpText);
}

/*void Effects::DrawBossBar(sf::RenderWindow& window)
{
	window.draw(bossHPLine);
	window.draw(bossBar);
}*/

void Effects::DrawWin(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	window.draw(win);
	window.draw(winText);
	window.draw(helpText);
}

void Effects::DrawAnnouncement(sf::RenderWindow& window)
{
	for (auto announcement : damageAnnouncementText)
	{
		window.draw(announcement);
	}
}

Cloud::Cloud(string const& text)
	:remark(text)
{
	cloudTexture.loadFromFile("resources/cloud.png");
	font.loadFromFile("files/fo.ttf");

	cloud.setTexture(&cloudTexture);
	cloud.setSize(REMARK_CLOUD_SIZE);
	cloud.setOrigin(REMARK_CLOUD_SIZE.x / 2.0f, REMARK_CLOUD_VERTICAL_ORIGIN);

	remarkText = sf::Text(remark, font, REMARK_FONT_SIZE);
	remarkText.setOrigin(remarkText.getGlobalBounds().width / 2.0f, remarkText.getGlobalBounds().height / 2.0f);
	remarkText.setFillColor(sf::Color::Black);
}

void Cloud::Update(sf::Vector2f const& position, float elapsedTime)
{
	cloud.setPosition(position + REMARK_CLOUD_MARGIN);
	remarkText.setPosition(cloud.getPosition() + REMARK_TEXT_MARGIN);
	duration += elapsedTime;

	cloud.setFillColor(Extinguish(cloud.getFillColor(), duration, REMARK_DURATION));
	remarkText.setFillColor(Extinguish(remarkText.getFillColor(), duration, REMARK_DURATION));
}

void Cloud::Draw(sf::RenderWindow& window)
{
	window.draw(cloud);
	window.draw(remarkText);
}

string IntToStr(int number)
{
	if (number >= 0)
	{
		return to_string(number);
	}
	else
	{
		return "0";
	}
}

sf::Vector2f GetTextureSize(sf::Texture const& texture)
{
	const sf::Vector2f size = {
		static_cast<float>(texture.getSize().x),
		static_cast<float>(texture.getSize().y)
	};

	return size;
}

sf::Color Extinguish(
	sf::Color const& color,
	float elementDuration,
	float maxDuration,
	float extinguishDuration
	)
{
	float startExtinguishTime = maxDuration - extinguishDuration;
	sf::Color newColor;
	if (elementDuration >= startExtinguishTime)
	{
		float extinguishElapsedTime = elementDuration - startExtinguishTime;
		sf::Uint8 transStep = static_cast<sf::Uint8>(255 * extinguishElapsedTime / extinguishDuration);
		newColor = sf::Color(255, 255, 255, 255 - transStep);

		return newColor;
	}

	return color;
}