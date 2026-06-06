#include <SFML/Graphics.hpp>
#include "game.h"
#include "EndScreen.h"
#include <windows.h>
using namespace sf;

float scale = 1.0f;
float volume = 100.0f; // Domyślna wartość głośności

void loadSettings() {
	std::ifstream inFile("settings.txt");
	if (inFile.is_open()) {
		std::string line;
		while (std::getline(inFile, line)) {
			if (line.find("scale:") == 0) {
				scale = std::stof(line.substr(6));
			}
			else if (line.find("volume:") == 0) {
				volume=std::stof(line.substr(7));
			}
		}
		inFile.close();
	}
	else {
		std::cerr << "Nie udało się otworzyć pliku ustawień!" << std::endl;
	}
}

int main() {

	loadSettings(); // Ładowanie ustawień z pliku
    int windowWidth = float(1280*scale);
    int windowHeight = float(800 * scale);

    RenderWindow window(VideoMode(windowWidth, windowHeight), "2D Battle Simulator",Style::Close);
	EndScreen endScreen(window, scale);
	Game game(window, scale); // Inicjalizacja gry z przekazaniem okna i skali
	game.setVolume(volume);
	game.setScale(scale, window); // Ustawienie skali gry




    //zmiana kursora
    sf::Image cursorImage;
    if (!cursorImage.loadFromFile("graphics/UI/cursor.png")) {
        return -1; // Sprawdzenie, czy udało się załadować plik
    }

    sf::Cursor cursor;
    if (cursor.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), { 0, 0 })) {
        window.setMouseCursor(cursor);
    }

 //  //muzyka
 //   sf::Music music;
	//if (!music.openFromFile("music/preparation_soundtrack.wav")) {
	//	std::cout << "Nie udało się załadować pliku dźwiękowego!" << std::endl;
	//	return -1;
	//}
	//music.play();


    while (window.isOpen()) {
        window.clear(sf::Color(0,120,0)); // Czyszczenie ekranu
		Sprite background;
		Texture backgroundTexture;
		if (!backgroundTexture.loadFromFile("graphics/mapa.png")) {
			std::cout << "Nie można załadować tekstury tła!" << std::endl;
		}
		background.setTexture(backgroundTexture);
		background.setScale(scale*2, scale*2);
		background.setPosition(0, 0);
		window.draw(background); // Rysowanie tła
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (game.gameStatus != otherWindow) {
			game.update(event, window);  // Aktualizacja logiki
			if (game.gameStatus == Status::reset) {
                game.ResetGame();
				game.gameStatus = Status::setting;
			}
			if (game.gameStatus == scaling) {
				scale = game.scale;
				windowWidth = int(1280 * scale);
				windowHeight = int(800 * scale);
				window.create(VideoMode(windowWidth, windowHeight), "2D Battle Simulator", Style::Close);
				endScreen.setScale(scale,window);
				game.setScale(scale, window);
				game.gameStatus = Status::setting;

			}

        }
        else
            endScreen.isActive = true;

        game.render(window);  // Renderowanie


		if (game.gameStatus == Status::goodWon || game.gameStatus == Status::badWon || game.gameStatus == Status::noWinner) {
			if (game.gameStatus == Status::goodWon) {
				endScreen.goodWons = true;
			}
			else {
				if (game.gameStatus == Status::noWinner) //jeśli nie ma zwycięzcy
					endScreen.noWinner = true;
				else
					endScreen.noWinner = false;
				endScreen.goodWons = false;
			}
            game.gameStatus = Status::otherWindow;
		}
		if (endScreen.isActive) {
			endScreen.Update(window,game.gameStatus,event);
			endScreen.render(window);
		}

        window.display();//wyswietlanie okna
    }

    return 0;
}
//notatki:




//Fajnie dodać:

//przesuwana mapa
//particle chodzenia
//dzwiek uderzenia
//krzyki w tle
//uwidocznienie ataku particlami
//przeskody i spowolnienia(kamienie, błoto, rzeki) 

//rycerz z atakiem obszarowym
//alchemmik z granatami
//artyleria
//ogr z armatą na ramieniu
//szaman-leczy
//dowódca- przyspiesza
//miotacz toporów


