#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <ctime>
#include <functional>
#include "unit.h"
#include "archer.h"
#include "horseman.h"
#include "projectile.h"
#include "button.h"
#include "unitSource.h"

class Game {
private:
    sf::RenderWindow& window;  // Referencja do okna
    sf::RectangleShape panel;    // Panel UI
    std::vector<sf::RectangleShape> otherUi;
    int idCounter;//iloœæ zespawnowanych jednostek
    std::vector<Button> buttons;//lista przycisków 
    std::string log;//zmienna przechowuj¹ca log
    std::string errors;//zmienna na wszystkie errory
    int time[2];
    Status previousStatus;
    unitPattern unitsPatterns[4];//wzorce jednostek do kopiowania
    int activePattern;//indeks jednostki do skopiowania
    bool activeSet;//czy obecnie mozna stawiaæ nowe jednostki
    int tick;
    int previousTick;
    sf::CircleShape blueprint;//wskaŸnik miejsca do postawienia jednostki
	sf::Font font;//czcionka do tekstu


    //dane blueprintu
    float size;

    int windowWidth;
    int windowHeight;
    //muzyka
    void PlayMusic();
	int musicClock; //licznik czasu do zmiany muzyki
    int toEnd=-1;
	sf::Music music;


    //dane gry
	//int maxPowerPoints = 200;//maksymalna iloœæ punktów, które mo¿na wykorzystaæ do stawiania jednostek
	int powerPoints[2] = { 0,0 }; //punkty, które mo¿na wykorzystaæ do stawiania jednostek,indeks 0 dla lewj strony, 1 dla prawej

	sf::Text powerPointsText[2]; //tekst z iloœci¹ punktów

    //funkcje pomocnicze:
    int findButtonByName(std::string name);//funkcja do znajdywania przycisku po nazwie, zwraca index z tablicy
    bool canSet(sf::Vector2f mousePos);//funkcja do sprawdzenia czy mo¿na postawiæ jednostkê
    float distance(float posX1, float posY1, float posX2, float posY2);//funckja do obliczania odleg³oœci
    void disactivePanel(std::string panelName);
    //std::string GetTexture(int activePattern, std::string team);

    //logika bitwy
    int delay;
    std::vector<std::unique_ptr<Unit>> units;    //lista jednostek na planszy
	std::vector<Projectile> projectiles; //lista pocisków

    sf::Color orange=sf::Color(254, 90, 4);
    sf::Color darkblue = sf::Color(34, 32, 52);

	void loadSettings(); //funkcja do wczytywania ustawieñ z pliku

public:
    Game(sf::RenderWindow& win,float tscale);
    void update(sf::Event& event,sf::RenderWindow& win);  // Aktualizacja logiki gry
    void render(sf::RenderWindow& win);  // Renderowanie gry
    void setWindowSize(int x, int y);
    void ResetGame();//metoda do resetowania gry
    Status gameStatus;
    float scale;
    void setScale(float tscale, sf::RenderWindow& win);
	void setVolume(float volume); //ustawia g³oœnoœæ muzyki
};

#endif // GAME_H
