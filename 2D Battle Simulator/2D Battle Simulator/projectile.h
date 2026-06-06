#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "unitVector.h"
#include <chrono>
#include <iostream>
#include "status.h"
class Projectile
{
private:
	int prevTime = -1;
	int time;


	float direction; 


	float damage;

	float scale;

	void loadTexture();

	std::vector<int> piercedId;//lista id jednostek któym ju¿ zada³ damage

	UnitVector vectorFunctions;
	sf::Sprite projectileSprite;
	sf::Texture projectileTexture;

	float borderX = 100000.0f;
	float borderY=100000.0f;
public:
	Projectile(float tdirection,float tposX,float tposY,float tspeed, float tdamage, float tsize,std::string tteam, float tscale);
	Projectile(float tdirection, float tposX, float tposY, float tspeed, float tdamage, float tsize, std::string tteam, float tscale, int tpiercing);

	void Update(Status gameStatus);
	void render(sf::RenderWindow& win);

	bool isPierced(int id);
	float getDamage(int id);

	void setScale(float tscale);

	float pierced;//ile pocisk mo¿e jeszcze przebiæ jednostek

	std::string team;//team pocisku, ¿eby nie trafia³ swoich
	float posX;
	float posY;
	float size;
	float speed;

};

