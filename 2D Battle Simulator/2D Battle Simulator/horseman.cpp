#include "horseman.h"
Horseman::Horseman(unitPattern unitData, float tposX, float tposY, std::string tteam, int tid, float tscale)
    : Unit(unitData, tposX, tposY, tteam, tid, tscale), damageBar(speed* (1 - multiplierInitialSpeed), tscale)
{
	realSpeed = speed * multiplierInitialSpeed;
    damageBar.SetBackgrundColor(sf::Color(200, 200, 200, 200));
    damageBar.SetColor(sf::Color(254, 90, 4));
	acceleration = unitData.acceleration;
	UnitSource unitSource=UnitSource();
    standardProjectileSpeed = unitSource.standardProjectileSpeed;
}

void Horseman::Command(std::vector<std::unique_ptr<Unit>>& units) {
    float minDistance = 100000;
    int minIndex = -1;
    float distance = 0;
    if (units.size() > 0) {
        for (int i = 0; i < units.size(); i++) {
                distance = vectorFunctions.checkDistance(units[i]->posX, units[i]->posY, this->posX, this->posY);
                if (units[i]->team != this->team) {
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = i;
                }//szukanie najbli¿szego wroga


            }
          
            //kolizje
            CheckCollision(i, distance, units);//z innymi jednostkami
        }


        if (minIndex == -1) {
            battleFinished = true;//jeœli nie zmieniono indeksu, nie ma wrogów, wiêc koñczy bitwê
        }
        else {

                distance = vectorFunctions.checkDistance(units[minIndex]->posX, units[minIndex]->posY, this->posX, this->posY);
                //std::cout << minDistance << "---x---" << this->size + myUnits[minIndex].size;
                direction = vectorFunctions.checkDirection(this->posX, this->posY, units[minIndex]->posX, units[minIndex]->posY);
                if (minDistance > this->size + units[minIndex]->size + range - 0.1f)
                {
                    addVector(7, direction, (realSpeed)*scale * float(deltaTimeMiS / 1000.0));
                    if (realSpeed >= speed)
                        realSpeed = speed;
                    else
					realSpeed += acceleration * float(deltaTimeMiS / 1000.0);

                    //przypisuje wektor w kierunku wroga
                }
                else {
                    addVector(7, direction, 0);
                    //jeœli wróg jest w zasiêgu to obraca w jego stronê
                }
            //atak


            if (minDistance < this->size + units[minIndex]->size + range) {
                //std::cout << minIndex<<":" << distance << "<--" << this->size + myUnits[minIndex].size;
                if (nextAttackTime <= time) {
                    attack = true;//ustawienie ataku
                    if (animCount == atackFrame) {//odpowiada za zgranie ataku z animacj¹
                        Attack(minIndex, units,damage*realSpeed/speed);//wykonanie ataku na jednostkê o indeksie minIndex
                        nextAttackTime = time + int(cooldown * 1000);//dodanie cooldownu zamienionego z sekund na milisekundy
						realSpeed = speed * multiplierInitialSpeed;//reset prêdkoœci
                    }

                }

            }


        }



    }

}
void Horseman::additionalRendering(sf::RenderWindow& win, Status GameStatus, float TopBorderY) {
	if (GameStatus == Status::running && realSpeed>speed*(multiplierInitialSpeed+0.05)) {
		damageBar.SetPos(posX - healthBar.width / 2 - 5*scale, posY - 20*scale);
        damageBar.SetProgress(realSpeed-multiplierInitialSpeed*speed);
		damageBar.render(win);
	}

}
void Horseman::projectileHit(int i, std::vector<Projectile>& projectiles) {
    projectiles[i].pierced--;
	realSpeed -= speed * 0.07f*projectiles[i].speed/standardProjectileSpeed;//zmniejszenie prêdkoœci po trafieniu
}