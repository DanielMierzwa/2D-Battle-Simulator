#pragma once
#include <SFML/Graphics.hpp>
class Slider
{
private:
	float progress;
	float posX;
	float posY;

	float scale;
	float maxProgress;

	sf::Color sliderColor = sf::Color::Green;
	sf::Color backgroundColor = sf::Color(60,60,60);
public:
	Slider(float tmaxProgress);
	Slider(float tmaxProgress,float tscale);
	void SetProgress(float tprogress);
	void SetScale(float tscale);
	void SetSize(float twidth, float theight);
	void SetPos(float tposX, float tposY);
	void render(sf::RenderWindow& win);
	void SetColor(sf::Color color);
	void SetBackgrundColor(sf::Color color);
	float width = 40.0f;
	float height = 3.0f;
};

