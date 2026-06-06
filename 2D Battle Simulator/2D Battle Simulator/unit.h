#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include "unitPattern.h"
#include "status.h"
#include "unitVector.h"
#include "slider.h"
#include "AnimationSource.h"
#include "status.h"
#include "projectile.h"
class Unit
{

	protected:
		int time;
		int prevTime;
		int deltaTimeMiS;//odstêp czasu miêdzy klatkami w milisekundach

		UnitVector vectorFunctions;
		std::string texture;// * 

		float borderX = -1.0;//prawo
		float borderY = -1.0;//dó³, okreœlaj¹ granice mapy, w której porusza siê jednostka
		float topBorderY = -1.0;//górna granica mapy, w której porusza siê jednostka

		float scale;
		float basicSize;// * 
		sf::Texture unitTexture;
		sf::Sprite unitSprite;
		std::string GetTexture(unitPattern activePattern, std::string team);
		Slider healthBar;
		//===
		std::string errors;
		std::string log;

		//std::vector<std::unique_ptr<Unit>>& myUnits;
		std::vector<Projectile> myProjectiles;

		//battle stats
		float maxHealth;// * 
		float speed;// * 
		float cooldown;// * 
		float range;// * 
		float damage;// * 

		//attacking
		int nextAttackTime;

		//dzwiek
		std::string attackSound;
		sf::Music soundEffects;

		//animations
		AnimationSource AnimS=AnimationSource();
		int animCount = 0;
		int animationSpeed = 500; //milisekundy miêdzy klatkami animacji
		int animationTick = 0; //licznik czasu animacji
		bool attack=false;
		int animIndex; // * //0-animacje dla w³ócznika, 1-animacje dla ³ucznika
		int atackFrame; // * //numer klatki animacji, w której jednostka wyzwala atak
		//funkcje porz¹dkowe====
		virtual void Command(std::vector<std::unique_ptr<Unit>>& units);//logika poruszania jednostki
		void Move();//wykonuje wektory
		void SaveData(std::vector<std::unique_ptr<Unit>>& units);//zapisuje dane do nastêpnej klatki
		virtual void Attack(int i, std::vector<std::unique_ptr<Unit>>& units,float d); //wykonuje atak na jednostkê o indeksie i, zadaje obra¿enia d
		void CheckCollision(int i,float distance, std::vector<std::unique_ptr<Unit>>& units);//sprawdza kolizje z innymi jednostkami
		void CheckHit();//sprawdza czy pocisk uderzy³ w jednostkê i zadaje obra¿enia
		virtual void additionalRendering(sf::RenderWindow& win, Status GameStatus, float TopBorderY);//dodatkowe renderowanie jednostki, np. animacje
		virtual void projectileHit(int i, std::vector<Projectile>& projectiles);//specjalne efekty po trafieniu pociskiem, funckja dla konnicy
		//przyjmuje index sprawdzanej jendostki i dystans od niej
	public:
		//funkcje====
		Unit(unitPattern unitData,float tposX,float tposY, std::string tteam,int tid,float tscale);
		void Update(Status gameStatus, std::vector<std::unique_ptr<Unit>>& units, std::vector<Projectile> projectiles);
		void render(sf::RenderWindow& win,Status GameStatus,float TopBorderY);

		void setVolume(float volume);
		std::vector<std::unique_ptr<Unit>> updateUnits();
		std::vector<Projectile> updateProjectiles();
		void setDirection(int x);
		void setScale(float tscale);
		void addVector(int tduration, float tdirection, float tforce);
		void dealDamage(float damage);
		std::string returnLog(std::string previousLog);
		std::string returnErrors(std::string previousErrors);

		//zmienne====
		float posX;//pozycja jednostki
		float posY;
		float size;
		float direction;
		bool battleFinished = false;
		std::vector<UnitVector> vectors;
		sf::CircleShape hitbox;//deklaracja hitboxy jednostki
		std::string team;
		int id;
		std::string typeName;// * 


		//battle stats
		float health;

		virtual ~Unit() = default;


		//*pobierany z unitPattern
};

