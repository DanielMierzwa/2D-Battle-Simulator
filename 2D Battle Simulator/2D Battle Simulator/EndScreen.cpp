#include "EndScreen.h"
using namespace sf;
EndScreen::EndScreen(sf::RenderWindow& win, float tscale) {
	scale = tscale;
	width = 700.0f;
	height = 200.0f;
	buttons.push_back(Button(0, 0, 200.0, 50.0, "zamknij", "ekran_końcowy", false));
	buttons.back().setTexture("graphics/UI/Buttons/zamknij.png", 5.0f, 5.0f); // Ustawia teksturę przycisku zamknij
    if (!font.loadFromFile("fonts/FiveFontsatFreddy's-Regular.ttf")) {
        // Błąd ładowania czcionki
        std::cout << "Nie można załadować czcionki!" << std::endl;
    }

    text.setCharacterSize(fontSize * scale); // w pikselach
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(win.getSize().x / 2 - offsetX * scale, win.getSize().y / 2-offsetY*scale);
    posX = win.getSize().x / 2 - width * scale / 2;
    posY = win.getSize().y / 2 - height * scale / 2;
}

void EndScreen::Update(sf::RenderWindow& win, Status& GameStatus, sf::Event event){
	// Update logic for the end screen can be added here
    buttons[findButtonByName("zamknij")].setPos(win.getSize().x/2/scale-100.0, win.getSize().y/2/scale-30.0f); // ustawia pozycję guzika zamknij na środku ekranu końcowego
	buttons[findButtonByName("zamknij")].setScale(scale); // ustawia skalę guzika zamknij
    Vector2i tempMousePos = Mouse::getPosition(win); // pobiera pozycję myszy
    Vector2f mousePos = win.mapPixelToCoords(tempMousePos);
    if (event.type == Event::MouseButtonPressed) { // gdy kliknięto myszą

        for (int i = 0; i < buttons.size(); i++) {//pętla obsługująca kliknięcia w guziki
            //sprawdza czy kliknieto przycisk
            //if (buttons[i].checkClick(mousePos)) {
            //    if (buttons[i].release) {
            //        disactivePanel(buttons[i].panelName);
            //        buttons[i].clickButton();
            //    }
            //    else {
            //        buttons[i].isActive = false;
            //    }

            //}
            buttons[i].checkClick(mousePos, [this](std::string name) {
                disactivePanel(name);
                });
            if (buttons[i].name == "zamknij" && buttons[i].isActive) {
                GameStatus = Status::reset; // restart gry
                isActive = false; // dezaktywacja ekranu końcowego
            }

        }
    }
    for (int i = 0; i < buttons.size(); i++) {//odskakiwanie przycisków "nie checkboxów"
        if (!buttons[i].checkbox)
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                buttons[i].resetButton();
            }

    }




    text.setFont(font);
    if (goodWons) // jeśli dobra drużyna wygrała{
    {
        text.setString("Zwyciestwo zielonych!");
        if (!texture.loadFromFile("graphics/UI/ekran_koncowy_zieloni.png")) {
            std::cout << "Nie można załadować tekstury zwycięstwa zielonych!" << std::endl;
        }
        
    }
	else if(noWinner)
    {		
        text.setString("Brak zwyciezcy!");//jesli po obu stronachzgineli wszyscy
        text.setPosition(win.getSize().x / 2 - 140.0f * scale, win.getSize().y / 2 - 50.0f * scale);
        if (!texture.loadFromFile("graphics/UI/ekran_koncowy_remis.png")) {
            std::cout << "Nie można załadować tekstury braku zwycięzcy!" << std::endl;
        }
    }
    else {
        text.setString("Zwyciestwo fioletowych!");// jeśli zła drużyna wygrała
        if (!texture.loadFromFile("graphics/UI/ekran_koncowy_fioletowi.png")) {
            std::cout << "Nie można załadować tekstury zwycięstwa różowych!" << std::endl;
        }
    }



}

void EndScreen::render(sf::RenderWindow& win) {
	
	RectangleShape screen(Vector2f(width*scale,height*scale));
	screen.setPosition(posX, posY);
	screen.setTexture(&texture);
	win.draw(screen);
    for (int i = 0;i < buttons.size();i++) {
        buttons[i].setScale(scale);
        buttons[i].render(win);
    }
    win.draw(text);
}

int EndScreen::findButtonByName(std::string name) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].name == name) {
			return i;
		}

	}
}
void EndScreen::disactivePanel(std::string panelName) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].panelName == panelName) {
            buttons[i].resetButton();
        }
    }
}
void EndScreen::setScale(float tscale, sf::RenderWindow& win) {
    scale = tscale;
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].setScale(scale);
    }
    text.setCharacterSize(fontSize * scale); // w pikselach
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(win.getSize().x / 2 - offsetX * scale, win.getSize().y / 2 - offsetY * scale);
    posX = win.getSize().x / 2 - width * scale / 2;
    posY = win.getSize().y / 2 - height * scale / 2;
}