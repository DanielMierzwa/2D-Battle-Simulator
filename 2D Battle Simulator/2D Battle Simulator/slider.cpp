#include "slider.h"
using namespace sf;
Slider::Slider(float tmaxProgress,float tscale)  {
	SetScale(tscale);
	SetProgress(tmaxProgress);
	maxProgress = tmaxProgress;
}
void Slider::SetProgress(float tprogress) {
	if (tprogress < 0) {
		progress = 0;
	}
	else if (tprogress > maxProgress) {
		progress = maxProgress;
	}
	else {
		progress = tprogress;
	}
}
void Slider::SetPos(float tposX, float tposY) {
	posX = tposX;
	posY = tposY;
}
void Slider::SetScale(float tscale) {
	this->scale = tscale;
}
void Slider::SetSize(float twidth, float theight) {
	width = twidth;
	height = theight;
}
void Slider::render(sf::RenderWindow& win) {

	RectangleShape background(Vector2f(width*scale, height*scale));
	background.setPosition(posX, posY);
	RectangleShape slider(Vector2f(width * scale * progress / maxProgress, height * scale));
	slider.setPosition(posX, posY);
	slider.setFillColor(sliderColor);
	background.setFillColor(backgroundColor);
	win.draw(background);
	win.draw(slider);

}

void Slider::SetColor(sf::Color color) {
	sliderColor = color;
}

void Slider::SetBackgrundColor(sf::Color color) {
	backgroundColor = color;
}