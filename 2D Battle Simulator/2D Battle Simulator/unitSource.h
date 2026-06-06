#pragma once
#include "unitPattern.h"

class UnitSource
{
public:
    //atakframe nie mo¿e byæ ostatni¹ klatk¹ animacji ataku
    //jeœli nie dzia³aj¹ nowe animacje, upewniæ siê ¿e animIndex jest prawid³owy, a nie np. równy d³ugoœci tablicy
    unitPattern unitsPatterns[4];
	float standardProjectileSpeed = 100.0f; //standardowa prêdkoœæ pocisku
    UnitSource() {
        //definicja wzorów jednostek
   //defincja rycerza
        unitsPatterns[0].size = 20.0f;
        unitsPatterns[0].typeName = "rycerz";
        unitsPatterns[0].texture = "wlocznik1.png";
        unitsPatterns[0].speed = 2.5f;
        unitsPatterns[0].health = 100.0f;
        unitsPatterns[0].damage = 20.0f;
        unitsPatterns[0].range = 5.0f;
        unitsPatterns[0].cooldown = 2.0f;
        unitsPatterns[0].powerPointsCost = 20;
        unitsPatterns[0].animIndex = 0;
        unitsPatterns[0].attackFrame = 5;
        unitsPatterns[0].attackSound = "cios.wav";
        //defincija ³ucznika
        unitsPatterns[1].size = 20.0f;
        unitsPatterns[1].typeName = "lucznik";
        unitsPatterns[1].texture = "lucznik.png";
        unitsPatterns[1].speed = 2.5f;
        unitsPatterns[1].health = 80.0f;
        unitsPatterns[1].damage = 10.0f;
        unitsPatterns[1].range = 500.0f;
        unitsPatterns[1].cooldown = 2.0f;
        unitsPatterns[1].piercingOfProjectile = 1; // ³ucznik przebija 1 jednostkê
		unitsPatterns[1].projectileSpeed = standardProjectileSpeed; // prêdkoœæ pocisku ³ucznika
        unitsPatterns[1].powerPointsCost = 15;
        unitsPatterns[1].animIndex = 1;
        unitsPatterns[1].attackFrame = 5;
        unitsPatterns[1].attackSound = "strzal.wav";

        //defincija kusznika
        unitsPatterns[2].size = 20.0f;
        unitsPatterns[2].typeName = "kusznik";
        unitsPatterns[2].texture = "kusznik.png";
        unitsPatterns[2].speed = 2.5f;
        unitsPatterns[2].health = 80.0f;
        unitsPatterns[2].damage = 15.0f;
        unitsPatterns[2].range = 900.0f;
        unitsPatterns[2].cooldown = 3.0f;
        unitsPatterns[2].piercingOfProjectile = 20; // kusznik powinien przebiæ wszytskie jednostki
		unitsPatterns[2].projectileSpeed = standardProjectileSpeed*1.5f; // prêdkoœæ pocisku kusznika
        unitsPatterns[2].powerPointsCost = 30;
        unitsPatterns[2].animIndex = 3;
        unitsPatterns[2].attackFrame = 6;
        unitsPatterns[2].attackSound = "strzal.wav";

        //defincja jezdzca
        unitsPatterns[3].size = 24.0f;
        unitsPatterns[3].typeName = "jezdziec";
        unitsPatterns[3].texture = "jezdziec.png";
        unitsPatterns[3].speed = 10.0f;
        unitsPatterns[3].health = 150.0f;
		unitsPatterns[3].damage = 120.0f;//maksymalna wartoœæ obra¿eñ, przy pe³nej prêdkoœci
        unitsPatterns[3].range = 5.0f;
        unitsPatterns[3].cooldown = 3.0f;
        unitsPatterns[3].powerPointsCost = 30;
        unitsPatterns[3].piercingOfProjectile = 0;//w³ócznik nie strzela wiêc jest 0
        unitsPatterns[3].animIndex = 2;
        unitsPatterns[3].attackFrame = 4;
		unitsPatterns[3].acceleration = 1.0f; //przyspieszenie jeŸdŸca
        unitsPatterns[3].attackSound = "cios_jezdziec.wav";
    };
};
