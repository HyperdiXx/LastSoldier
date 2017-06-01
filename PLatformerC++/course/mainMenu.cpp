#include "mainmenu.h"
#include <iostream>


using namespace std;
using namespace sf;


Menu::Menu(float width, float height):resolution(width, height)
{
	gameNameTexture.loadFromFile("files/images/gameLogo.png");
	buttonFont.loadFromFile("files/fo.ttf");
	mainWrapperTexture.loadFromFile("files/images/mainmenu.png");
	iconTexture.loadFromFile("files/images/eagle1.png");
	pauseWrapperTexture.loadFromFile("files/images/pauseMenu.png");
	ABOUT.loadFromFile("files/images/info.png");

	gameName.setSize(GAMENAME_SIZE);
	gameName.setOrigin(GAMENAME_SIZE.x / 2.0f, GAMENAME_SIZE.y);
	gameName.setTexture(&gameNameTexture);

	menuIcon.setSize(ICON_SIZE);
	menuIcon.setTexture(&iconTexture);
	menuIcon.setOrigin(ICON_SIZE * 0.5f);
	menuIcon.setScale(2.f, 0.6f);
	

	mainMenuButtons = {
		sf::Text("Start", buttonFont, FONT_SIZE),
		sf::Text("About", buttonFont, FONT_SIZE),
		sf::Text("Leave", buttonFont, FONT_SIZE)
	};

	aboutMenuButtons = 
	{
		sf::Text("back", buttonFont, FONT_SIZE)
	};

	pauseMenuButtons = {
		sf::Text("Continue", buttonFont, FONT_SIZE_MEDIUM),
		sf::Text("Restart", buttonFont, FONT_SIZE_MEDIUM),
		sf::Text("Main menu", buttonFont, FONT_SIZE_MEDIUM)};

	resizeMenuButtons = {
		sf::Text("1024 x 576", buttonFont, FONT_SIZE_MEDIUM),
		sf::Text("1280 x 720", buttonFont, FONT_SIZE_MEDIUM),
		sf::Text("1366 x 768", buttonFont, FONT_SIZE_MEDIUM),
		sf::Text("1920 x 1080", buttonFont, FONT_SIZE_MEDIUM)};

	allMenues = {
		mainMenuButtons,
		aboutMenuButtons,
		pauseMenuButtons,
		resizeMenuButtons
	};

	for (auto menu = allMenues.begin(); menu != allMenues.end(); menu++)
	{
		for (auto button = menu->begin(); button != menu->end(); button++)
		{
			sf::Vector2f origin(button->getGlobalBounds().width, button->getGlobalBounds().height);
			button->setOrigin(origin * 0.5f);
		}
	}
}

void Menu::SetMenu(MenuType const& menu, sf::Vector2f const& center)
{
	currentMenu = menu;
	currentButton = 0;
	buttonsColdown = 0;

	if (menu != MenuType::PAUSE)
	{
		gameName.setPosition(
			center.x,
			center.y - resolution.y / 2.0f + GAMENAME_VERTICAL_MARGIN
			);
	}

	float margin = MENU_FIRST_BUTTON_MARGIN;
	auto currMenu = static_cast<size_t>(currentMenu);

	for (auto it = allMenues[currMenu].begin(); it != allMenues[currMenu].end(); it++)
	{
		it->setPosition(center.x, center.y + margin);
		margin += MENU_BUTTONS_MARGIN;
	}

	if (menu == MenuType::PAUSE)
	{
		menuWrapper.setSize(static_cast<sf::Vector2f>(PAUSE_MENU_SIZE));
		menuWrapper.setTexture(&pauseWrapperTexture);
		menuWrapper.setTextureRect(sf::IntRect({ 0, 0 }, PAUSE_MENU_SIZE));
	}
	else if (menu == MenuType::START || menu == MenuType::ABOUT)
	{
		menuWrapper.setSize(resolution);
		menuWrapper.setTexture(&mainWrapperTexture);
		menuWrapper.setTextureRect(sf::IntRect({ 0, 0 }, MAIN_MENU_WRAPPER_SIZE));
	}
	else if (menu == MenuType::RESIZE_SETTINGS)
	{
		menuWrapper.setSize(resolution);
		menuWrapper.setFillColor(RESIZE_SETTINGS_BG_COLOR);
	}

	sf::Vector2f origin(menuWrapper.getGlobalBounds().width, menuWrapper.getGlobalBounds().height);
	menuWrapper.setOrigin(origin * 0.5f);
	menuWrapper.setPosition(center);
}

void Menu::Update(sf::Vector2f const& center)
{
	auto currMenu = static_cast<size_t>(currentMenu);
	auto currItem = allMenues[currMenu][currentButton];

	auto menuButtonWidth = currItem.getGlobalBounds().width;
	auto menuButtonCenterY = currItem.getGlobalBounds().height / 2.0f + currItem.getPosition().y;
	float currIconMargin = menuButtonWidth / 2.0f + ICON_HORIZONTAL_MARGIN;

	menuIcon.setPosition({ center.x - currIconMargin , menuButtonCenterY });
}

void Menu::Select(MenuType const& selectMenu, Difficult const& selectButton)
{
	size_t selectMenuId = static_cast<size_t>(selectMenu);
	size_t selectButtonId = static_cast<size_t>(selectButton);

	for (size_t buttonNum = 0; buttonNum < allMenues[selectMenuId].size() - 1; buttonNum++)
	{
		if (buttonNum != selectButtonId)
		{
			allMenues[selectMenuId][buttonNum].setFillColor(UNSELECTED_ITEM_COLOR);
		}
		else
		{
			allMenues[selectMenuId][buttonNum].setFillColor(SELECTED_BUTTON_COLOR);
		}
	}
}

void Menu::SwitchButtonUp()
{
	auto currMenu = allMenues[(size_t)currentMenu];
	auto maxItem = currMenu.size() - 1;

	buttonsColdown = 0;
	currentButton = (currentButton == 0) ? maxItem : currentButton - 1;
}

void Menu::SwitchButtonDown()
{
	auto currMenu = allMenues[(size_t)currentMenu];
	auto maxItem = currMenu.size() - 1;

	buttonsColdown = 0;
	currentButton = (currentButton == maxItem) ? 0 : currentButton + 1;
}

void Menu::About(sf::RenderWindow& window)
{
	ABOUT.loadFromFile("files/images/info.png");
	Sprite i(ABOUT);
	i.setPosition(500, 700);
	window.draw(i);
	window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape));
}
void Menu::Draw(sf::RenderWindow& window)
{
	window.draw(menuWrapper);

	if (currentMenu != MenuType::PAUSE)
	{
		window.draw(gameName);
	}

	auto currMenu = allMenues[(size_t)currentMenu];
	for (auto menuElement : currMenu)
	{
		window.draw(menuElement);
	}
	window.draw(menuIcon);
}