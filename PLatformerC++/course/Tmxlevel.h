#pragma once

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class TmxObject
{
public:
	TmxObject() {};
	~TmxObject() {};
	int GetPropertyInt(std::string propertyName);
	float GetPropertyFloat(std::string propertyName);
	std::string GetPropertyString(std::string propertyName);

	void MoveBy(const sf::Vector2f &movement);
	void MoveTo(const sf::Vector2f &position);

	std::string name;
	std::string type;
	sf::FloatRect rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

class TmxLayer
{
public:
	TmxLayer() {};
	~TmxLayer() {};
	sf::Uint8 opacity = 0;
	std::vector<sf::Sprite> tiles;
};

class Tmxlevel
{
public:
	
	Tmxlevel() {};
	~Tmxlevel() {};

	bool LoadFromFile(const std::string &filepath);

	TmxObject GetFirstObject(const std::string &name)const;
	std::vector<TmxObject> GetAllObjects(const std::string &name)const;
	sf::Vector2i GetTileSize()const;
	float GetTilemapWidth()const;
	float GetTilemapHeight()const;
	sf::Vector2f GetTilemapSize()const;

	void Draw(sf::RenderWindow& window, sf::FloatRect const& drawArea)const;

private:
	int m_width = 0;
	int m_height = 0;
	int m_tileWidth = 0;
	int m_tileHeight = 0;
	int m_firstTileID = 0;
	sf::Texture m_tilesetImage;
	std::vector<TmxObject> m_objects;
	std::vector<TmxLayer> m_layers;
}; 
