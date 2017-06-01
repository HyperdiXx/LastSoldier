
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <limits>
#include "game.h"
#include "Authorization.h"

using namespace std;
using namespace sf;


static const string GAME_NAME = "LAST SOLDIER";
static const sf::Vector2u START_RESOLUTION = { 1280, 720 };
static const float CRITICAL_ELAPSED_TIME = 0.1f;
static const float NORMAL_ELAPSED_TIME = 0.016f;


void InitScenes(Game& game);
void InitGamePlayScene(Game& game);
void InitMenuScene(Game& game);
void InitGameOverScene(Game& game);
void InitPreviewScene(Game& game);
void InitWinScene(Game& game);

void EnterGameLoop(sf::RenderWindow& window, Game& game);

void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game);
void Render(sf::RenderWindow& window, Game& game);
bool enter(string &login, string &password);
void registration(string &login, string &password);
void RUN();
void start(string &login);
string XOR(string data, char key[]);



string XOR(string data, char key[])
{
	string xorstring = data;
	for (int i = 0; i < xorstring.size(); i++)
	{
		xorstring[i] = data[i] ^ key[i % (sizeof(key) / sizeof(char))];
	}
	return xorstring;
}



class Character
{
	
public:
		char name[16];
		char pword[10];
		char p2[10];
		char key[3] = { 'K', 'E', 'Y' };
		void sendNAME();
		void sendPWORD();
		//string getPWORD(char a[10], char k[3]) const;
		//string temp1;
		string data = XOR(pword, key);
		//string undata = XOR(p2,key);
};

/*string Character::getPWORD(char a[10], char k[3]) const
{
	return bdata = XOR(p2,key);
}*/

void Character::sendNAME()
{
	cout << "Enter your desired username(max 16 characters): ";
	cin >> name;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
};


void Character::sendPWORD()
{
		cout << "Enter your password(max 10 characters): ";
		cin >> pword;
		//char key[3] = { 'K', 'E', 'Y' };
		//string data = XOR(pword, key);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
};



int startMenu()

{
		int option;
		cout << "WELCOME TO LOGIN MENU!!!\n";
		cout << "1.Log in\n2.Register\n3.PLAY FOR FREE\n4.Exit\n";
		cout << "Your option: ";
		cin >> option;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return option;
	
};



int main()
{
	char key[3] = { 'K', 'E', 'Y' };
	ifstream readFile("example.txt");
	ofstream writeFile("example.txt", ios::app);
	Character charInfo;
	string username;
	string password;
	string temp;
	string temp1;
	do
	{
		int option = startMenu();
		cout << endl;
		if (option == 1)
		{
			if (readFile.is_open())
			{
				
				cout << "Enter Name: ";
				cin >> username;
				cout << "Enter Password: ";
				cin >> password;
				cout << endl;
				readFile >> temp;
				cout << "Scanning for username...";
				if (username == temp)
				{
					cout << "\nUsername found, checking password" << endl;
					
					readFile >> temp1;
					//string undata = XOR(temp1, key);

					if (password == temp1)
					{
						cout << "Welcome " << username << "!" << endl;
						RUN();
						break;
					}

				}
				else
				{

					cout << "Invalid username or password." << endl;

				}
				readFile.close();
			}
			break;
		}
		cout << endl;
		if (option == 2)
		{
			charInfo.sendNAME();
			cout << endl;
			charInfo.sendPWORD();
			cout << endl;
			if (writeFile.is_open())
			{
				writeFile << charInfo.name << endl;
				writeFile << charInfo.pword << endl;
				writeFile.close();
			}
			else
			{
				cout << "Unable to open file";
			}
			break;
		}
		if (option == 3)
		{
			RUN();
			break;
		}
		if (option == 4)
		{
			exit(1);
		}
	} while (1);
	return 0;
}



void EnterGameLoop(sf::RenderWindow& window, Game& game)
{
	while (window.isOpen())
	{
		game.SetElapsedTime();

		if (game.elapsedTime >= CRITICAL_ELAPSED_TIME)
		{
			game.elapsedTime = NORMAL_ELAPSED_TIME;
		}

		HandleEvents(window, game);
		Update(game);
		Render(window, game);
	}
}

void HandleEvents(sf::RenderWindow& window, Game& game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
	game.currentScene->toHandle(window);
}

void Update(Game& game)
{
	game.UpdateOST();
	game.currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(BACKGROUND_COLOR);
	window.draw(game.visualEffects.currentBackground);
	game.currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
	InitGamePlayScene(game);
	InitMenuScene(game);
	InitGameOverScene(game);
	InitPreviewScene(game);
	InitWinScene(game);

	game.currentScene = &game.previewScene;
}

void InitGamePlayScene(Game& game)
{
	game.gameplayScene.toHandle = [&](sf::RenderWindow& window) {
		window;
		game.ControlPlayer();
	};
	game.gameplayScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.UpdatePlayer();
		game.UpdateEnemies();
		game.UpdateBullets();
		game.UpdateTemporaryObjects();
		game.UpdateBonuses();
		game.CheckEntitiesCollides();
	};
	game.gameplayScene.onDraw = [&](sf::RenderWindow& window) {
		game.UpdateCamera(window);
		game.UpdateInterface();
		game.DrawLevel(window);
		game.player.DrawCharacter(window);
		game.DrawBullets(window);
		game.DrawEnemies(window);
		game.DrawBonuses(window);
		game.DrawTemporaryObjects(window);
		game.DrawInterface(window);
	};
}

void InitMenuScene(Game& game)
{
	game.menuScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlMenu(window);
	};
	game.menuScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.menu.Update(game.camera.getCenter());
	};
	game.menuScene.onDraw = [&](sf::RenderWindow& window) {
		if (game.menu.currentMenu == MenuType::PAUSE)
		{
			game.gameplayScene.onDraw(window);
		}
		window.setView(game.camera);
		game.menu.Draw(window);
	};
}

void InitGameOverScene(Game& game)
{
	game.gameOverScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlGameOver(window);
	};
	game.gameOverScene.onUpdate = [&]() {
		sf::RectangleShape* gameOverBack = &game.visualEffects.gameOver;
		auto pos = game.camera.getCenter();

		gameOverBack->setPosition(pos);
	};
	game.gameOverScene.onDraw = [&](sf::RenderWindow& window) {
		window.draw(game.visualEffects.gameOver);
	};
}

void InitPreviewScene(Game& game)
{
	game.previewScene.toHandle = [&](sf::RenderWindow& window) {
		(void)window;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			game.currentScene = &game.menuScene;
		}
	};
	game.previewScene.onUpdate = [&]() {
		auto cameraCenter = game.camera.getCenter();
		if (game.visualEffects.UpdatePreview(cameraCenter, game.elapsedTime))
		{
			game.currentScene = &game.menuScene;
		}
	};
	game.previewScene.onDraw = [&](sf::RenderWindow& window) {
		game.visualEffects.DrawPart(window);
	};
}

void InitWinScene(Game& game)
{
	game.winScene.toHandle = [&](sf::RenderWindow& window) {
		(void)window;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			game.menu.SetMenu(MenuType::START, game.camera.getCenter());
			game.currentScene = &game.menuScene;
		}
	};
	game.winScene.onUpdate = [&]() {
		auto cameraCenter = game.camera.getCenter();
		game.visualEffects.UpdateWin(cameraCenter);
	};
	game.winScene.onDraw = [&](sf::RenderWindow& window) {
		game.visualEffects.DrawWin(window);
	};
}

bool enter(string &login, string &password) {
	cout << "Enter ur login:" << endl;
	try {
		cin.ignore();
		getline(cin, login);
		cout << "Enter ur password:" << endl;
		getline(cin, password);
		if (login.empty() || password.empty())
			throw login;
	}
	catch (string) {
		if (login.empty())
			cout << "Login is empty" << endl;
		if (password.empty())
			cout << "Password is empty" << endl;
		throw;
	}
	Authorization usr;
	return usr.enter(login, password);
}
void registration(string &login, string &password) {
	cout << "Enter ur login:" << endl;
	try {
		cin.ignore();
		getline(cin, login);
		cout << "Enter ur password:" << endl;
		getline(cin, password);
		if (login.empty() || password.empty())
			throw login;
	}
	catch (string) {
		if (login.empty())
			cout << "Login is empty" << endl;
		if (password.empty())
			cout << "Password is empty" << endl;
		throw;
	}
	Authorization usr;
	usr.registration(login, password);
}
void RUN()
{
	sf::VideoMode gameVideoMode;
	gameVideoMode.width = START_RESOLUTION.x;
	gameVideoMode.height = START_RESOLUTION.y;
	sf::RenderWindow gameWindow(gameVideoMode, GAME_NAME, sf::Style::Titlebar | sf::Style::Close);

	Game game(static_cast<float>(START_RESOLUTION.x), static_cast<float>(START_RESOLUTION.y));

	InitScenes(game);
	EnterGameLoop(gameWindow, game);

}
void start(string &login)
{
	Authorization usr;
	if (usr.access(login) == 0)
		RUN();
	if (usr.access(login) == 1)
		RUN();
}

