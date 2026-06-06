#include "archer.h"
Archer::Archer(unitPattern unitData, float tposX, float tposY, std::string tteam, int tid, float tscale)
    : Unit(unitData, tposX, tposY, tteam, tid, tscale)
{
	piercingOfProjectile = unitData.piercingOfProjectile;
	projectileSpeed = unitData.projectileSpeed;
}
void Archer::Attack(int i, std::vector<std::unique_ptr<Unit>>& units,float d) {
	myProjectiles.push_back(Projectile(
		vectorFunctions.checkDirection(posX, posY, units[i]->posX, units[i]->posY),
		posX, posY, projectileSpeed, d, 2.0f,team, scale,piercingOfProjectile));
}