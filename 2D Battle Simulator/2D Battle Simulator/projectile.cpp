#include "projectile.h"
Projectile::Projectile(float tdirection, float tposX, float tposY, float tspeed, float tdamage, float tsize, std::string tteam, float tscale) : vectorFunctions(0.0, 0.0, 0.0) {
	speed = tspeed;
	direction = tdirection;
	posX = tposX;
	posY = tposY;
	damage = tdamage;
	pierced = 1;
	scale = tscale;
	size = tsize;
	team = tteam;
	loadTexture();
}

Projectile::Projectile(float tdirection, float tposX, float tposY, float tspeed, float tdamage, float tsize, std::string tteam, float tscale, int tpiercing) : vectorFunctions(0.0, 0.0, 0.0) {
	speed = tspeed;
	direction = tdirection;
	posX = tposX;
	posY = tposY;
	damage = tdamage;
	pierced = tpiercing;
	scale = tscale;
	size = tsize;
	team = tteam;
	loadTexture();
}
void Projectile::loadTexture() {
	if (!projectileTexture.loadFromFile("graphics/strzala.png")) {  // Replace with your actual image file path
		std::cout << "Error loading texture!: graphics/strzala.png\n";
	}
	else {
		projectileSprite.setTexture(projectileTexture);
	}
}

void Projectile::Update(Status gameStatus) {
	auto now = std::chrono::system_clock::now();

	// obliczanie czasu
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()
	).count();
	time = millis % 1000000000;
	if (prevTime < 0) {
		prevTime = time;
	}
	float DeltaTime = (time - prevTime) / 1000.0;//zmiana czasu od ostatniej klatki w sekundach
	prevTime = time;

	if (gameStatus != Status::running) {
		return;
	}

	//ruch pocisku
	UnitVector vector(1,direction,speed*DeltaTime*scale);
	posX += vector.move.x;
	posY += vector.move.y;

	if (posX < 0 || posX > borderX || posY < 0 || posY > borderY) {
		//pocisk wyszed≥ poza ekran
		pierced = 0;
		return;
	}

}

void Projectile::render(sf::RenderWindow& win) {
	//sf::RectangleShape shape;
	//shape.setPosition(posX, posY);
	//shape.setSize(sf::Vector2f(size * scale, size * scale));
	//shape.setFillColor(sf::Color(255, 0, 0, 255)); // Ustawienie koloru przezroczystego

	projectileSprite.setScale(scale, scale);
	projectileSprite.setPosition(posX, posY);
	projectileSprite.setTexture(projectileTexture);
	projectileSprite.setOrigin(projectileTexture.getSize().x , 0);

	projectileSprite.setRotation(-direction+135); 
	//win.draw(shape); // Rysowanie prostokπta jako wskaünika
	win.draw(projectileSprite);
	borderX = win.getSize().x;
	borderY = win.getSize().y;
}

bool Projectile::isPierced(int id) {
	for (int i = 0; i < piercedId.size(); i++) {
		if (piercedId[i] == id) {
			return true;
		}
	}
	return false;
}

float Projectile::getDamage(int id) {
	piercedId.push_back(id);
	return damage;
}

void Projectile::setScale(float tscale) {
	posX /= scale;
	posY /= scale;
	scale = tscale;
	posX *= scale;
	posY *= scale;
}