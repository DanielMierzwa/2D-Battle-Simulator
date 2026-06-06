#pragma once
#include "unit.h"
#include "unitSource.h"
enum State { charge, turning, retreat };
class Horseman : public Unit
{
private:
	void Command(std::vector<std::unique_ptr<Unit>>& units) override;
	void additionalRendering(sf::RenderWindow& win, Status GameStatus, float TopBorderY) override;
	void projectileHit(int i, std::vector<Projectile>& projectiles) override;

	float multiplierInitialSpeed = 0.3f;//mnozniik podanej prêdkoœci, abyustaliæ prêdkoœæ z której konnica bêdzie przyspieszaæ
	float realSpeed;
	float acceleration;//* // przyspieszenie konnicy
	float standardProjectileSpeed; //standardowa prêdkoœæ pocisku, jest pobierana z unitSource.h w celu obliczenia spowolnienia konnicy przez pociski
	Slider damageBar; // pasek obra¿eñ
	public:
		Horseman(unitPattern unitData, float tposX, float tposY, std::string tteam, int tid, float tscale);

};

