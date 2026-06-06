#pragma once
#include "unit.h"
class Archer : public Unit
{
private:
	int piercingOfProjectile; //* //liczba jednostek, które mo¿e przebiæ strza³a
	void Attack(int i, std::vector<std::unique_ptr<Unit>>& units,float d) override;
	float projectileSpeed; //* //prêdkoœæ pocisku
public:
	Archer(unitPattern unitData, float tposX, float tposY, std::string tteam, int tid, float tscale);


};

