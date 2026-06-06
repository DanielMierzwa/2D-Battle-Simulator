#include <string>
#pragma once
struct unitPattern {
	std::string typeName;
	std::string texture;
	float damage;
	float health;
	float speed;
	float size;
	float range;
	float cooldown;
	int powerPointsCost;
	std::string attackSound;
	//informacje o animacji
	int animIndex;//0-animacje dla w³ócznika, 1-animacje dla ³ucznika
	int attackFrame; //numer klatki animacji, w której jednostka wyzwala atak
	//informacje dla szczególnego rodzaju jednostek
	int piercingOfProjectile;//ile wrogów mog¹ przebiæ pociski zanim znikn¹
	float projectileSpeed;//prêdkoœæ pocisku, tylko dla ³ucznika i kusznika
	float acceleration;//przyspieszenie, tylko dla konnicy

};
