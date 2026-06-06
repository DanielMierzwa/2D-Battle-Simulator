#include "game.h"
using namespace sf;
Game::Game(RenderWindow& win, float tscale) : window(win) ,panel(Vector2f(window.getSize().x, (window.getSize().y*0.2f) ) ),blueprint(20.0f){
    //pobranie danych jednostek
    UnitSource unitSource=UnitSource();
    for (int i=0; i < std::size(unitsPatterns);i++)//kopiowanie wzorców jednostek do listy
    {
        unitsPatterns[i] = unitSource.unitsPatterns[i];
    }
    idCounter = 0;
    previousTick = 0;
    tick = 0;//resetowanie ticków
    time[0] = int(std::time(nullptr));//pobieranie czasu na poczatku
    activeSet = false;
    gameStatus = Status::setting;//domyślny tryb to stawianie jednostek
    scale = tscale;
    //domyślny blueprint
    size = 20.0f;
    activePattern = 0;

    delay = 0;

    //inicjalizacja panelu
    panel.setPosition(0.0f, 0.0f);
    panel.setFillColor(darkblue);
	panel.setScale(scale, scale);//skalowanie panelu

	otherUi.push_back(RectangleShape());
    otherUi.push_back(RectangleShape());

    float height = 50.0f * scale;
    otherUi[0].setSize(Vector2f(win.getSize().x, height));
    otherUi[0].setPosition(0.0f, panel.getSize().y- height);

    otherUi[1].setPosition(0.0f, panel.getSize().y - height);

    float centerOfButton = 25.0f;
    float padding = 100.0f;
    float fontSize = 15.0f;

	//przyciski do stawiania jednostek
    buttons.push_back(Button(10.0,10.0,200.0,50.0,"wlocznik","przyciski_stawiania", true));
    buttons.back().setText("Wlocznik", 10.0f, centerOfButton,fontSize);
    buttons.back().setText(std::to_string(unitsPatterns[0].powerPointsCost), padding, centerOfButton, orange);
    buttons.back().setTexture("graphics/UI/Buttons/wlocznik_przycisk.png", 5.0f, 5.0f);

    buttons.push_back(Button(220.0, 10.0, 200.0, 50.0, "lucznik", "przyciski_stawiania", true));
    buttons.back().setText("Lucznik", 10.0f, centerOfButton, fontSize);
    buttons.back().setText(std::to_string(unitsPatterns[1].powerPointsCost), padding, centerOfButton, orange);
    buttons.back().setTexture("graphics/UI/Buttons/lucznik_przycisk.png", 5.0f, 5.0f);

    buttons.push_back(Button(430.0, 10.0, 200.0, 50.0, "kusznik", "przyciski_stawiania", true));
    buttons.back().setText("Kusznik", 10.0f, centerOfButton, fontSize);
    buttons.back().setText(std::to_string(unitsPatterns[2].powerPointsCost), padding, centerOfButton,orange);
    buttons.back().setTexture("graphics/UI/Buttons/kusznik_przycisk.png", 5.0f, 5.0f);

    buttons.push_back(Button(640.0, 10.0, 200.0, 50.0, "jezdziec", "przyciski_stawiania", true));
    buttons.back().setText("Husarz", 10.0f, centerOfButton, fontSize);
    buttons.back().setText(std::to_string(unitsPatterns[3].powerPointsCost), padding, centerOfButton, orange);
    buttons.back().setTexture("graphics/UI/Buttons/jezdziec_przycisk.png", 5.0f, 5.0f);

    buttons.push_back(Button(1040, 10.0, 50.0, 50.0, "dzwiek", "menu_gry", false));//reset
    buttons.back().setTexture("graphics/UI/Buttons/wlaczany_dzwiek.png", 5.0f, 5.0f);

    buttons.push_back(Button(1100, 10.0, 50.0, 50.0, "zmienRozmiar", "menu_gry", false));//play
    buttons.back().setTexture("graphics/UI/Buttons/powieksz.png", 5.0f, 5.0f);

    buttons.push_back(Button(1160, 10.0, 50.0, 50.0, "reset", "menu_gry", false));//reset
    buttons.back().setTexture("graphics/UI/Buttons/reset.png", 5.0f, 5.0f);

    buttons.push_back(Button(1220, 10.0, 50.0, 50.0, "play", "menu_gry", false));//play
    buttons.back().setTexture("graphics/UI/Buttons/play.png", 5.0f, 5.0f);

   

    if (!music.openFromFile("music/preparation_soundtrack.wav")) {
        std::cout << "Nie udało się załadować pliku dźwiękowego!" << std::endl;
        return;
    }

    if (!font.loadFromFile("fonts/FiveFontsatFreddy's-Regular.ttf")) {
        // Błąd ładowania czcionki
        std::cout << "Nie można załadować czcionki!" << std::endl;
    }

    //dane gry
    powerPoints[0] = 0;
    powerPoints[1] = 0; //punkty, które można wykorzystać do stawiania jednostek
}

void Game::update(Event& event,sf::RenderWindow& win) {

    musicClock = int(std::time(nullptr));
	PlayMusic(); // odtwarzanie muzyki, jeśli jest włączona
    // Tu można dodać logikę gry, np. poruszanie obiektami
    time[1] = int(std::time(nullptr));//pobieranie aktualnego czasu
    if (time[1] - time[0] > 5) {
        time[1] = time[0];
        previousTick = tick;
    }

    previousStatus = gameStatus;//zapamiętanie poprzedniego statusu gry

    Vector2i tempMousePos = Mouse::getPosition(win); // pobiera pozycję myszy
    Vector2f mousePos = win.mapPixelToCoords(tempMousePos);
	bool settingsChanged = false;//jeśli w tym renderze nikt nie kliknął guzika żeby zmienić ustawienia, to nie trzeba ich zapisywać
//    log += "mouse pos: " + std::to_string(mousePos.x) + "," + std::to_string(mousePos.y) + "\n";
//    log += "active unit setting: " + std::to_string(activeSet) + "\n";
//    log += "game status: " + std::to_string(gameStatus) + "\n";

    //obsługa wskaźnika
    if (activeSet == true && gameStatus == Status::setting) {

        blueprint.setPosition(Vector2f(mousePos.x - size * scale, mousePos.y - size * scale));
        blueprint.setRadius(size);
        if (canSet(mousePos)) {
            if (mousePos.x > win.getSize().x / 2)
                blueprint.setFillColor(Color(255, 30, 127, 140));
            else
                blueprint.setFillColor(Color(0, 234, 0, 140));
        }
        else {
            blueprint.setFillColor(Color(orange.r, orange.g, orange.b, 140));
        }
    }
    // obsługa eventów
    if (event.type == Event::MouseButtonPressed || event.type==Event::KeyReleased && event.key.code == sf::Keyboard::Tab) { // gdy kliknięto myszą,albo użyto skrótu klawiszowego
        
        if (activeSet == true && canSet(mousePos) && event.type == Event::MouseButtonPressed && gameStatus == Status::setting) {//stawianie jednostki, jesli stawianie jest aktywne i jednostka z niczym nie koliduje
            std::string team;
            if (mousePos.x < win.getSize().x / 2)
                team = "good";
            else
                team = "bad";
                if (activePattern == 0)//jeśli aktywny wzorzec to wlocznik
                {
                    units.push_back(std::make_unique<Unit>(unitsPatterns[activePattern], mousePos.x, mousePos.y, team, idCounter, scale));//dodaje wlocznika do listy
                }
                else if (activePattern == 1 || activePattern == 2)//jeśli aktywny wzorzec to łucznik
                {
				 units.push_back(std::make_unique<Archer>(unitsPatterns[activePattern], mousePos.x, mousePos.y, team, idCounter, scale));//dodaje łucznika do listy
                }
                else if (activePattern == 3) 
                {
					units.push_back(std::make_unique<Horseman>(unitsPatterns[activePattern], mousePos.x, mousePos.y, team, idCounter, scale));//dodaje kawalerzystę do listy
                }
                if (team == "bad")
                    units[units.size()-1]->setDirection(270);
                else
                    units[units.size()-1]->setDirection(90);
                units.back()->setVolume(music.getVolume());//ustawienie dzwieku jednostki

                idCounter++;
                //activeSet = false;
			    powerPoints[team == "good" ? 0 : 1] += unitsPatterns[activePattern].powerPointsCost;//odejmuje punkty za jednostkę
                //disactivePanel("przyciski_stawiania");
            
                
            
        }
        //skróty klawiszowe do przycisków
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab) {
                buttons[findButtonByName("zmienRozmiar")].isActive = true;
            }

        if (gameStatus != Status::otherWindow)
        
        for (int i = 0; i < buttons.size(); i++) {//pętla obsługująca kliknięcia w guziki
            //sprawdza czy kliknieto przycisk
            buttons[i].checkClick(mousePos, [this](std::string name) {
                disactivePanel(name);
                });
            //if (buttons[i].checkClick(mousePos)) {
            //    if (buttons[i].release) {
            //        disactivePanel(buttons[i].panelName);
            //        buttons[i].clickButton();
            //    }
            //    else {
            //        buttons[i].isActive = false;
            //    }

            //}

            if (gameStatus == Status::setting) {
                
                
                //przyciski do stawiania jednostek
                if (buttons[i].name == "wlocznik") {
                    if (buttons[i].isActive) {
                    
                        activeSet = true;
                        activePattern = 0;
                        size = unitsPatterns[activePattern].size;
                    }
                }
                if (buttons[i].name == "lucznik") {
                    if (buttons[i].isActive) {
                        activeSet = true;
                        activePattern = 1;
                        size = unitsPatterns[activePattern].size;
                    
                    }
                }
                if (buttons[i].name == "kusznik") {
                    if (buttons[i].isActive) {
                        activeSet = true;
                        activePattern = 2;
                        size = unitsPatterns[activePattern].size;

                    }
                }
                if (buttons[i].name == "jezdziec") {
                    if (buttons[i].isActive) {
                        activeSet = true;
                        activePattern = 3;
                        size = unitsPatterns[activePattern].size;

                    }
                }
            }

            if (buttons[i].name == "play") {
                    if (buttons[i].isActive) {
						settingsChanged = true;
                        if (gameStatus == Status::setting)
                        {
                            gameStatus = Status::running;
                        }
                        else
                            gameStatus = Status::setting;

                    }
                }
            if (buttons[i].name == "reset") {
                if (buttons[i].isActive) {
					ResetGame();

                }
            }
            if (buttons[i].name == "zmienRozmiar") {
                if (buttons[i].isActive && gameStatus!=Status::scaling) {
                    settingsChanged = true;
                    if (scale == 1.4f){
                        scale = 1.0f;
                        buttons[i].isActive = false;
                    }
                    else {
                        scale = 1.4f;
					}
                        gameStatus = Status::scaling;

                }
            }
            if (buttons[i].name == "dzwiek") {
                if (buttons[i].isActive) {
                    settingsChanged = true;
                    buttons[i].clearTextures();
                    if (music.getVolume() == 0.0f) {
                        setVolume(100.0f);
                    }
                    else {
                        setVolume(0.0f);
                    }
                }

            }

        }
    }
    for (int i = 0; i < buttons.size(); i++) {//odskakiwanie przycisków "nie checkboxów"
        if (!buttons[i].checkbox)
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                buttons[i].resetButton();
            }
                
    }
    if (settingsChanged)
    {
        //zapisanie ustawień
        std::ofstream outFile("settings.txt");
        std::string settings = "scale:" + std::to_string(scale) + "\n";
        settings += "volume:" + std::to_string(music.getVolume()) + "\n";
        if (outFile.is_open()) {
            // Zapisujemy zmienną do pliku
            outFile << settings << std::endl;  // Zapisujemy tekst do pliku
            outFile.close();  // Zamykamy plik po zapisaniu
        }
        else {
            std::cerr << "Nie udało się otworzyć pliku!" << std::endl;
        }
    }


    


    int goodSurvivors = 0;
    int badSurvivors = 0;

    //usuwanie martwych
    int deleted = 0;//ile w tym update usunięto już jednostek

    for (int i = 0; i < units.size(); i++) {
        //units.erase(units.begin() + died[i]);
        if (units[i]->health <= 0) {
            if (units.size() > 1) {
                units.erase(units.begin() + i - deleted);
                deleted++;
            }

        }
        else {
            if (units[i]->team == "good")
                goodSurvivors += 1;
            else
                badSurvivors += 1;
        }
    }

    //wyświetlanie ilości punktów obu stron lub jednostek po obu stronach

    powerPointsText[0].setFont(font);
    powerPointsText[0].setCharacterSize(42*scale); // w pikselach
    powerPointsText[0].setFillColor(sf::Color(255, 255, 255));
    powerPointsText[0].setPosition(0.0f, panel.getSize().y*scale - (20.0f * scale));

    powerPointsText[1].setFont(font);
    powerPointsText[1].setCharacterSize(42*scale); // w pikselach
    powerPointsText[1].setFillColor(sf::Color(255, 255, 255));

    if (gameStatus == Status::setting) {
        powerPointsText[0].setString(std::to_string(powerPoints[0]));
        powerPointsText[1].setString(std::to_string(powerPoints[1]));

        float rightOffset;
		if (powerPoints[1] < 10)//jeśli punktów jest mniej niż 10, to przesuwa tekst w lewo
			rightOffset = 30.0f;
		else if (powerPoints[1] < 100)
			rightOffset = 60.0f;
		else
			rightOffset = 90.0f ;

        powerPointsText[1].setPosition(win.getSize().x - (rightOffset * scale), panel.getSize().y* scale - (20.0f * scale));
        otherUi[0].setFillColor(Color(80, 80, 80));
		otherUi[1].setFillColor(Color(80, 80, 80,0));
    }
    if (gameStatus == Status::running) {
        //powerPointsText[0].setString(std::to_string(goodSurvivors)+"#");
        //powerPointsText[1].setString(std::to_string(badSurvivors) + "#");
        powerPointsText[0].setString("");
        powerPointsText[1].setString("");
        powerPointsText[1].setPosition(win.getSize().x - (100.0f * scale), panel.getSize().y - (20.0f * scale));
        otherUi[0].setFillColor(Color::Magenta);
        otherUi[1].setFillColor(Color::Green);
        float height = 50.0f*scale;
        if (badSurvivors == 0)
            otherUi[1].setSize(Vector2f(win.getSize().x, height));
        else if(goodSurvivors==0)
			otherUi[1].setSize(Vector2f(0.0f, height));
        else
        otherUi[1].setSize(Vector2f(win.getSize().x * goodSurvivors / (badSurvivors + goodSurvivors), height));

    }

    

    tick++;
        if ((badSurvivors == 0 || goodSurvivors == 0)&&gameStatus==Status::running) {
            delay += 1;
        }
        if (delay == 4) {
			errors += "\n" + std::to_string(tick) + "badSurvivors:" + std::to_string(badSurvivors) + " goodSurvivors:" + std::to_string(goodSurvivors);
            if (badSurvivors > 0)
                gameStatus = Status::badWon;
            if (goodSurvivors > 0)
                gameStatus = Status::goodWon;
            if (badSurvivors == 0 && goodSurvivors == 0)
                gameStatus = Status::noWinner;
            delay = 0;
        }
        //logika bitwy
//        log += "\nUnits:";
        for (int i = 0; i < units.size(); i++) {
//            log += "\nteam: " + units[i]->team + " | direction: " + std::to_string(units[i]->direction);
            units[i]->Update(gameStatus, units,projectiles);
			projectiles = units[i]->updateProjectiles();   
        }
		//logika pocisków
        int deletedProjectiles = 0;
		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i-deletedProjectiles].Update(gameStatus);
			//usuwanie pocisków które trafiły ustaloną ilość jednostek, lub wyleciały poza ekran
			if (projectiles[i].pierced <= 0) {
                if (projectiles.size() > 1) {
                    projectiles.erase(projectiles.begin() + i - deletedProjectiles);
				    deletedProjectiles++;
                }
				
			}
		}
			PlayMusic();


}

void Game::render(sf::RenderWindow& win) {

	//linia podziału planszy
    if (gameStatus == Status::setting) {
        RectangleShape line(Vector2f(16.0f, win.getSize().y));
	    line.setPosition(win.getSize().x / 2-8.0f, 0.0f);
	    line.setFillColor(Color(60,60,60,90));
        win.draw(line);//rysowanie linii podziału planszy
    }
	




    
    for (int i = 0; i < units.size(); i++) {//pętla do rysowania jednostek
        units[i]->setScale(scale);
        units[i]->render(win,gameStatus,panel.getSize().y);
    }
    if (activeSet && gameStatus == Status::setting)//jesli jest aktywne stawianie wyświetl blueprint
    {
        blueprint.setScale(scale, scale);
        win.draw(blueprint);
    }
	for (int i = 0; i < projectiles.size(); i++) {//pętla do rysowania pocisków
		projectiles[i].setScale(scale);
		projectiles[i].render(win);
	}
    
    win.draw(this->panel);
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i].setScale(scale);
            if(buttons[i].panelName=="przyciski_stawiania" && gameStatus == Status::setting)
                buttons[i].render(win);
            if(buttons[i].panelName == "menu_gry")
                buttons[i].render(win);
            //log += buttons[i].name + " | active: " + (buttons[i].isActive ? "true" : "false") + " | checkbox: "+ (buttons[i].checkbox ? "true" : "false") + " | release: " + (buttons[i].release ? "true" : "false") + "\n";
        }
		for (int i = 0; i < otherUi.size(); i++) {//pętla do rysowania innych elementów UI
			win.draw(otherUi[i]);
		}
        win.draw(powerPointsText[0]);
		win.draw(powerPointsText[1]);//wyświetlanie ilości punktów

    


    //log += "\n\nERRORS:" + errors;
    //std::cout << log;//wyswietlenie logu
}
void Game::setWindowSize(int x, int y) {
    windowWidth = x;
    windowHeight = y;
}

void Game::PlayMusic() {
    //muzyka

    if (previousStatus != gameStatus) {
        bool definedStatus = false;
        if (gameStatus == Status::setting) {
            music.stop(); // zatrzymanie muzyki przy zmianie statusu
            toEnd = -1; // resetowanie czasu do końca muzyki
			definedStatus = true;
            if (!music.openFromFile("music/preparation_soundtrack.wav")) {
                std::cout << "Nie udało się załadować pliku dźwiękowego!" << std::endl;
                return;
            }
        }
        else if (gameStatus == Status::running) {
            music.stop(); // zatrzymanie muzyki przy zmianie statusu
            toEnd = -1; // resetowanie czasu do końca muzyki
            definedStatus = true;
            if (!music.openFromFile("music/battle_soundtrack.wav")) {
                return;
            }
        }
        if (!definedStatus) {
            return; // jeśli status nie został zdefiniowany, nie zmieniaj muzyki
        }
    }

		if (toEnd>musicClock && toEnd!=-1)//jeśli muzyka jest już odtwarzana, to nie odtwarzaj ponownie
        {
            return;
        }
        music.play();
		toEnd = musicClock + static_cast<int>(music.getDuration().asSeconds());

}

void Game::ResetGame() {
    units.clear();
	projectiles.clear();//czyszczenie listy jednostek i pocisków
	idCounter = 0; //resetowanie licznika jednostek
	activeSet = false; //resetowanie aktywnego stawiania
	gameStatus = Status::setting;//resetowanie statusu gry
    powerPoints[0] = 0;
    powerPoints[1] = 0;  //resetowanie punktów
}

//funkcje pomocnicze
int Game::findButtonByName(std::string name) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].name == name) {
            return i;
        }
        
   }
    errors+= "\n" + std::to_string(tick) + ":(ERROR)Nie znaleziono przycisku o nazwie : " + name ;
        return 0;
}

bool Game::canSet(Vector2f mousePos) {
    if (panel.getGlobalBounds().contains(mousePos))//sprawdza kolizje z panelem UI
        return false;
    for (int i = 0; i < units.size(); i++) {//sprawdza kolizje z innymi jednostkami
        if (units[i]->hitbox.getRadius()+size > distance(mousePos.x, mousePos.y, units[i]->posX, units[i]->posY))
            return false;
    }
    return true;
}

float Game::distance(float posX1, float posY1, float posX2, float posY2) {
    float a = abs(posX1 - posX2);
    float b = abs(posY1 - posY2);
    return sqrt(a * a + b * b);
}

void Game::disactivePanel(std::string panelName) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].panelName == panelName) {
            buttons[i].resetButton();
        }
        if (panelName == "przyciski_stawiania") {
            activeSet = false;
        }
    }
}

void Game::setScale(float tscale, sf::RenderWindow& win) {
	scale = tscale;
	for (int i = 0;i < units.size();i++) {
		units[i]->setScale(scale);
	}
    for (int i = 0;i < buttons.size();i++) {
        buttons[i].setScale(scale);
    }
    float height = 50.0f * scale;
    errors += "\nheight:" + std::to_string(height);
    otherUi[0].setSize(Vector2f(win.getSize().x, height));
    otherUi[0].setPosition(0.0f, panel.getSize().y * scale - height);

    otherUi[1].setPosition(0.0f, panel.getSize().y * scale - height);

    panel.setScale(scale, scale);

    //dostosowanie wyglądu przycisku
	int i = findButtonByName("zmienRozmiar");
    buttons[i].clearTextures();

    if (scale == 1.4f) {
        buttons[i].setTexture("graphics/UI/Buttons/zmniejsz.png", 5.0f, 5.0f);
    }
    else {
        buttons[i].setTexture("graphics/UI/Buttons/powieksz.png", 5.0f, 5.0f);
    }
}

void Game::loadSettings() {
    std::ifstream inFile("settings.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line.find("scale:") == 0) {
                scale = std::stof(line.substr(6));
				errors += "\nscale:" + std::to_string(std::stof(line.substr(6)));
            }
            else if (line.find("volume:") == 0) {
                music.setVolume(std::stof(line.substr(7)));
            }
        }
        inFile.close();
    }
    else {
        std::cerr << "Nie udało się otworzyć pliku ustawień!" << std::endl;
    }
}
void Game::setVolume(float volume) {
	music.setVolume(volume);
	int i = findButtonByName("dzwiek");
			buttons[i].clearTextures();
			if (volume == 0.0f) {
				buttons[i].setTexture("graphics/UI/Buttons/wylaczany_dzwiek.png", 5.0f, 5.0f);
			}
			else {
				buttons[i].setTexture("graphics/UI/Buttons/wlaczany_dzwiek.png", 5.0f, 5.0f);
			}
    for (int i = 0; i < units.size(); i++) {
        units[i]->setVolume(volume);
    }
}
//std::string Game::GetTexture(int activePattern, std::string team) {
//    std:: string x="graphics/" + team + "/" + unitsPatterns[activePattern].texture;
//
//    return x;
//}


