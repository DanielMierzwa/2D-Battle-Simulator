#include "button.h"
using namespace sf;
Button::Button(float tposX, float tposY, float sizeX, float sizeY, std::string tname, std::string tpanelName, bool tcheckbox): hitbox(Vector2f(sizeX,sizeY)) {
	hitbox.setPosition(tposX, tposY);
	posX = tposX;
	posY = tposY;
	scale = 1.0f;
	name = tname;
	isActive = false;
	release = true;
	checkbox = tcheckbox;
	panelName = tpanelName;
	if (!font.loadFromFile("fonts/FiveFontsatFreddy's-Regular.ttf")) {
		// Błąd ładowania czcionki
		std::cout << "Nie można załadować czcionki!" << std::endl;
	}
}
void Button::checkClick(Vector2f mousePos, std::function<void(std::string)> disactivePanel) {
	bool checkClick = true;
		if (hitbox.getGlobalBounds().contains(mousePos))
			checkClick = true;
		else
			checkClick = false;
	if (checkClick) {
		//if (release) {
			disactivePanel(panelName);
			isActive = !isActive;
			release = false;
		//}
		//else {
		//	isActive = false;
		//}

	}
}
void Button::render(RenderWindow& win) {
    hitbox.setScale(scale, scale);
    hitbox.setPosition(posX * scale, posY * scale);

    if (!release)
        hitbox.setFillColor(Color(254, 90, 4));
    else
        hitbox.setFillColor(Color(165,165,165));

    win.draw(hitbox);

    for (int i = 0; i < textures.size(); i++) {
        sf::Sprite sprite;
        sprite.setTexture(textures[i]);

        sprite.setPosition(
            (posX + texturePos[i].x) * scale,
            (posY + texturePos[i].y) * scale
        );

        sprite.setScale(scale, scale);
        win.draw(sprite);
    }

    for (int i = 0; i < texts.size(); i++) {
        texts[i].setPosition(
            (posX + textPos[i].x) * scale,
            (posY + textPos[i].y) * scale
        );

        texts[i].setScale(scale, scale);
        texts[i].setFont(font);
        win.draw(texts[i]);
    }
}
void Button::resetButton() {
	isActive = false;
	release = true;
}
//void Button::clickButton() {
//	if (isActive)
//		isActive = false;
//	else
//		isActive = true;
//	release = false;
//}
void Button::setScale(float tscale) {
	scale = tscale;
}
void Button::setPos(float posX, float posY) {
	this->posX = posX;
	this-> posY = posY;
}

void Button::setFont(Font& sfont) {
	font = sfont;
}

void Button::setText(std::string text, float x, float y) {
	sf::Text newText;
	newText.setString(text);
	newText.setCharacterSize(20 * scale);
	newText.setFillColor(sf::Color::White);
	newText.setPosition((posX + x) * scale, (posY + y) * scale);
	texts.push_back(newText);
	textPos.push_back(pos{x,y});
}
void Button::setText(std::string text, float x, float y, sf::Color color) {
	setText(text,  x,  y);
	texts.back().setFillColor(color);
}
void Button::setText(std::string text, float x, float y, int fontSize) {
	setText(text,  x,  y);
	texts.back().setCharacterSize(fontSize * scale);
}
void Button::setText(std::string text, float x, float y, sf::Color color, int fontSize) {
	setText(text,  x,  y, color);
	texts.back().setCharacterSize(fontSize * scale);
}
void Button::setTexture(std::string texture, float x, float y) {
	sf::Texture tex;

	if (!tex.loadFromFile(texture)) {
		std::cout << "Nie można załadować tekstury: " << texture << std::endl;
		return;
	}

	textures.push_back(std::move(tex));
	texturePos.push_back(pos{ x, y });
}
void Button::clearTextures() {
	textures.clear();
	texturePos.clear();
}
