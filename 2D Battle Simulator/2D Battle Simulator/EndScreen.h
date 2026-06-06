#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <functional>
#include "status.h"
#include "button.h"
class EndScreen
{
private:
	float scale;
	float width;
	float height;

	float posX;
	float posY;

	std::vector<Button> buttons;
	int findButtonByName(std::string name);
	void disactivePanel(std::string panelName);
	sf::Text text;
	sf::Font font;
	int fontSize=30; //rozmiar czcionki
	float offsetX = 200.0f; //przesuniêcie tekstu w poziomie
	float offsetY = 50.0f; //przesuniêcie tekstu w pionie

	sf::Texture texture;
	
public:
	bool isActive = false; //czy ekran koñcowy jest aktywny
	bool goodWons = true; //czy dobra dru¿yna wygra³a
	bool noWinner = false; //czy nie ma zwyciêzcy
	EndScreen(sf::RenderWindow& win, float tscale);
	void setScale(float tscale, sf::RenderWindow& win);
	void Update(sf::RenderWindow& win, Status& GameStatus, sf::Event event);
	void render(sf::RenderWindow& win);
};

