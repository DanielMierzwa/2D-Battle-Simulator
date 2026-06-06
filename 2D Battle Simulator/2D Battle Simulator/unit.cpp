#include "unit.h"
using namespace sf;

Unit::Unit(unitPattern unitData, float tposX, float tposY, std::string tteam, int tid, float tscale) : hitbox(unitData.size), vectorFunctions(0.0, 0.0, 0.0),healthBar(unitData.health,tscale) {
    scale = 1.0f;
    size = unitData.size;
    basicSize = unitData.size;
    setScale(tscale);
    posX = tposX;
    posY = tposY;
    
    team = tteam;
    id = tid;
    texture = GetTexture(unitData,team);
    //battle stats
    maxHealth = unitData.health;
    health = maxHealth;
    speed = unitData.speed;
    cooldown = unitData.cooldown;
    range = unitData.range;
    damage = unitData.damage;
    typeName = unitData.typeName;
    //ustawienia hitboxu
    hitbox.setPosition(Vector2f(posX+size,posY+size));
    direction = -999;
    nextAttackTime = 0;
    if(team=="bad")
		healthBar.SetColor(Color(202,31,123,200));
	else
        healthBar.SetColor(Color(0, 255, 0,200));
	healthBar.SetBackgrundColor(Color(200, 200, 200, 200));

    
    if (team == "bad")
        hitbox.setFillColor(Color(255, 0, 127));
    else
        hitbox.setFillColor(Color(0, 204, 0));
    prevTime = -1;

    //definicja tekstury
// Load the texture for the unit
    if (!unitTexture.loadFromFile(texture)) {  // Replace with your actual image file path
        log += "Error loading texture!: " + texture + "\n";
    }
    else {
        unitSprite.setTexture(unitTexture);
    }

	//ustawienia animacji
	animIndex = unitData.animIndex;
	atackFrame = unitData.attackFrame;

    //dzwiek
    attackSound = "music/sound_effects/" + unitData.attackSound;

}
void Unit::Update(Status gameStatus, std::vector<std::unique_ptr<Unit>>& units, std::vector<Projectile> projectiles) {
    //myUnits = units;
	myProjectiles = projectiles;

    //pobieranie aktualnego czasu w milisekundach
    auto now = std::chrono::system_clock::now();

    // Przekonwertuj na czas od epoki w milisekundach
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
    time = millis % 1000000000;
	if (prevTime < 0) {
		prevTime = time;
	}
	deltaTimeMiS = time - prevTime; //ró¿nica czasu od ostatniej klatki w milisekundach
	float DeltaTime = (time - prevTime)/1000.0;//zmiana czasu od ostatniej klatki w sekundach

    log += "\n" + std::to_string(id) + ":" + typeName + "==========\n";
    if (gameStatus == Status::running) {

        Command(units);
        CheckHit();
		Move();
    }
    SaveData(units);

}
void Unit::render(RenderWindow& win,Status GameStatus,float TopBorderY) {
    //wyci¹ganie borderów z win
	borderX = float(win.getSize().x);
	borderY = float(win.getSize().y);//ustawienie granic mapy na rozmiar okna
	topBorderY = TopBorderY;//górna granica mapy, w której porusza siê jednostka
      //animacja
	int attackFrameCount = AnimS.attackAnim[animIndex][0].getFrameCount();
	int walkFrameCount = AnimS.walkAnim[animIndex][0].getFrameCount();
    if (GameStatus==Status::running) {
        if (attack) {
			animationSpeed = 150; //prêdkoœæ animacji ataku
            unitSprite = AnimS.attackAnim[animIndex][team == "good" ? 0 : 1].getFrame(animCount, unitSprite);
            if (animationTick >= animationSpeed) {
                animCount++;
                if (animCount == attackFrameCount) {
                    attack = false;//po zakoñczeniu animacji ataku, ustawienie ataku na false
                    if (soundEffects.openFromFile(attackSound)) {//dzwiêk ataku
                        soundEffects.play();
                    }
                }
                animCount = animCount % attackFrameCount;
                animationTick = 0;
            }
            animationTick += deltaTimeMiS;
        }
        else {
			animationSpeed = 500; //prêdkoœæ animacji ruchu
            unitSprite = AnimS.walkAnim[animIndex][team == "good" ? 0 : 1].getFrame(animCount, unitSprite);
            if (animationTick>=animationSpeed) {
                animCount++;
                animCount = animCount % walkFrameCount;
		    	animationTick = 0;
            }
		    animationTick += deltaTimeMiS;
        }
        
        
    }
    else {
		unitSprite = AnimS.walkAnim[animIndex][team == "good" ? 0 : 1].getFrame(0, unitSprite);
  //      if (!unitTexture.loadFromFile(texture)) {  // Replace with your actual image file path
  //          log += "Error loading texture!: " + texture + "\n";
		//}
		//unitSprite.setTexture(unitTexture);
		//i = 0; //resetowanie animacji
		//animationTick = 0;
    }
    
    unitSprite.setOrigin(32, 32);
   unitSprite.setPosition(posX, posY);
   unitSprite.setRotation(-direction+90);
   unitSprite.setScale(scale,scale);
   win.draw(unitSprite);
   //rysowanie healthbara
   if (GameStatus == Status::running && health > 0 && health < maxHealth) {
       healthBar.SetPos(posX- healthBar.width/2-5*scale, posY-15*scale);
       healthBar.render(win);
   }

   additionalRendering(win, GameStatus, TopBorderY);
   
}

//funckje porz¹dkowe====

void Unit::Command(std::vector<std::unique_ptr<Unit>>& units) {
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
            direction = vectorFunctions.checkDirection(this->posX, this->posY, units[minIndex]->posX, units[minIndex]->posY);
            if (minDistance > this->size + units[minIndex]->size+range-0.1f)
            {
                addVector(7, direction, (speed)*scale * float(deltaTimeMiS / 1000.0));
                //przypisuje wektor w kierunku wroga
            }
            else {
                addVector(7, direction, 0);
                //jeœli wróg jest w zasiêgu to obraca w jego stronê
            }
            //atak


            if (minDistance < this->size + units[minIndex]->size + range) {
                if (nextAttackTime <= time) {
                    attack = true;//ustawienie ataku
                    if (animCount == atackFrame) {//odpowiada za zgranie ataku z animacj¹


                        Attack(minIndex, units,damage);//wykonanie ataku na jednostkê o indeksie minIndex
                        nextAttackTime = time + int(cooldown * 1000);//dodanie cooldownu zamienionego z sekund na milisekundy
                    }
                }

            }


        }



    }
}

void Unit::Move() {
    if (vectors.size() != 0)//realizacja wektorów
    {
        for (int i = 0; i < vectors.size(); i++) {
            this->posX += vectors[i].move.x;
            this->posY += vectors[i].move.y;



            vectors[i].duration -= 1;
            // Sprawdzenie, czy duration wynosi 0 i usuniêcie wektora
            if (vectors[i].duration == 0) {
                vectors.erase(vectors.begin() + i);
                i--; // Decrementowanie indeksu, aby nie pomin¹æ nastêpnego elementu po usuniêciu

            }

        }

    }
    if (borderX != -1) {
        ////zabezpieczenie pred wyje¿d¿aniem poza mapê
        if (posX > borderX - size)
            posX = borderX - size;
        if (posY > borderY - size)
            posY = borderY - size;
        if (posX < size)
            posX = size;
        if (posY < size+topBorderY)
            posY = size+ topBorderY;
    }
	//aktualizacja hitboxu
    hitbox.setPosition(Vector2f((posX - size) * scale, (posY - size) * scale));
}
void Unit::SaveData(std::vector<std::unique_ptr<Unit>>& units) {
    prevTime = time;//zapisanie czasu z tej klatki dla nastêpnych powtórzeñ
    for (int i = 0; i < units.size(); i++) {
        if (units[i]->id == this->id) {
            units[i]->posX = this->posX;
            units[i]->posY = this->posY;
            units[i]->health = this->health;
            units[i]->direction = this->direction;
            units[i]->vectors = this->vectors;
            units[i]->nextAttackTime = this->nextAttackTime;
            units[i]->errors = this->errors;
            units[i]->log = this->log;
            units[i]->scale = this->scale;
            units[i]->prevTime = this->prevTime;
            units[i]->deltaTimeMiS = this->deltaTimeMiS;
            units[i]->attack = this->attack;
            // Dodaj inne w³aœciwoœci, które chcesz zaktualizowaæ
            break;
        }//aktualizacja jednostki w wektorze myUnits
    }
}

void Unit::Attack(int i, std::vector<std::unique_ptr<Unit>>& units,float d) {
    units[i]->dealDamage(d);
}

void Unit::CheckCollision(int i, float distance, std::vector<std::unique_ptr<Unit>>& units) {
    if (distance <= this->size + units[i]->size && this->id != units[i]->id) {
        float collisionforce = (this->size + units[i]->size - distance) / 2;

        float collisionDir = vectorFunctions.checkDirection(units[i]->posX, units[i]->posY, this->posX, this->posY);
        float enemyDir = direction; // wczeœniej ustawione jako kierunek do przeciwnika

        // Wektor bazowy w kierunku przeciwnika
        sf::Vector2f vec_enemy = vectorFunctions.directionToVector(enemyDir, speed * scale * float(deltaTimeMiS/1000.0));

        // Wektor kolizji
        sf::Vector2f vec_collision = vectorFunctions.directionToVector(collisionDir, collisionforce);

        // Wypadkowa
        sf::Vector2f vec_result = vec_enemy + vec_collision;

        // Ró¿nica
        sf::Vector2f vec_diff = vec_result - vec_enemy;

        float diff_mag = std::sqrt(vec_diff.x * vec_diff.x + vec_diff.y * vec_diff.y);

        // Dodaj ró¿nicê jako "boost" (dodatkowy wektor)
        float diff_dir = vectorFunctions.vectorToDirection(vec_diff);
        addVector(1, diff_dir, diff_mag);
    }
}

void Unit::CheckHit() {
	//sprawdzenie kolizji z pociskami
	for (int i = 0; i < myProjectiles.size(); i++) {
		if (myProjectiles[i].team != team) {//zabezpieczenie przed trafieniem swoich
            float distance = vectorFunctions.checkDistance(myProjectiles[i].posX, myProjectiles[i].posY, this->posX, this->posY);
            if (distance <= this->size + scale * myProjectiles[i].size) {
                if (!myProjectiles[i].isPierced(id)) {
                    dealDamage(myProjectiles[i].getDamage(id));
					projectileHit(i, myProjectiles);
                }
            }
        }
		
	}
}

void Unit::additionalRendering(RenderWindow& win, Status GameStatus, float TopBorderY) {
//funkcja do nadpisywania przez klasy pochodne, które bêd¹ chcia³y wyrenderowaæ dodatkowe rzeczy
    return;
}

void Unit::projectileHit(int i, std::vector<Projectile>& projectiles) {
	//funkcja do nadpisywania przez klasy pochodne, które bêd¹ chcia³y wykonaæ dodatkowe efekty po trafieniu pociskiem
	//domyœlnie:
    myProjectiles[i].pierced--;
}
//reszta funckji====
void Unit::setVolume(float volume) {
    soundEffects.setVolume(volume);
}

void Unit::addVector(int tduration, float tdirection, float tforce) {
    vectors.push_back(UnitVector(tduration, tdirection, tforce));
}
void Unit::dealDamage(float damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    healthBar.SetProgress(health);
}
void Unit::setDirection(int x) {
    direction = x;
}

void Unit::setScale(float tscale) {
    posX /= scale;
	posY /= scale;

    scale = tscale;

	posX *= scale;
	posY *= scale;
    size = basicSize * scale;
    hitbox.setRadius(size);

}
std::string Unit::GetTexture(unitPattern activePattern, std::string team) {
    std::string x = "graphics/" + team + "/" + activePattern.texture;

    return x;
}
std::string Unit::returnLog(std::string previousLog) {
    std::string tlog = log;
    log = "";
    return previousLog + tlog;
}
std::string Unit::returnErrors(std::string previousErrors) {
    std::string terrors = errors;
    errors = "";
    return previousErrors + errors;
}
//std::vector<std::unique_ptr<Unit>> Unit::updateUnits() {
//    return myUnits;
//}

std::vector<Projectile> Unit::updateProjectiles() {
    return myProjectiles;
}