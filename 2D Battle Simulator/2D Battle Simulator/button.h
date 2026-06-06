#pragma once
#include <iostream>
#include<SFML/Graphics.hpp>
#include <functional>
struct pos {
	float x;
	float y;
};
class Button
{
private:
	
	sf::RectangleShape hitbox;//przycisk
	float scale;
	float posX;
	float posY;
	std::vector<sf::Text> texts;//napisy na przycisku
	std::vector<pos> textPos;//pozycje wzglêdne napisów na przycisku
	//std::vector<std::string> textures;//adresy grafik na przycisku
	std::vector<sf::Texture> textures;
	std::vector<pos> texturePos;//pozycje wzglêdne grafik na przycisku

	sf::Font font;//czcionka przycisku

public:
	Button(float tposX, float tposY, float sizeX, float sizeY, std::string tname,std::string tpanelName,bool checkbox);
	std::string name;
	std::string panelName;
	void checkClick(sf::Vector2f mousePos, std::function<void(std::string)> disactivePanel);//funkcja obs³uguj¹ca klikniêcie w przycisk
	void render(sf::RenderWindow& win);//funkcja rysuj¹ca przycisk
	void resetButton();// do odklikiwania przycisku
	//void clickButton();// do klikania przycisku
	bool isActive;//czy przycisk jest aktywny
	bool checkbox;//typ przycisku: checkbox lub zwyk³y
	bool release;//czy puszczono przycisk
	void setScale(float tscale);
	void setPos(float posX, float posY);
	void setFont(sf::Font& sfont);

	void setText(std::string text, float x, float y);
	void setText(std::string text, float x, float y, sf::Color color);
	void setText(std::string text, float x, float y, int fontSize);
	void setText(std::string text, float x, float y, sf::Color color, int fontSize);

	void setTexture(std::string texture, float x, float y);
	void clearTextures();
};

