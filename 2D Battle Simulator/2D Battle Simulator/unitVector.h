#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
struct Move {
	float x, y;
	
};
class UnitVector {
	public:
		UnitVector(int tduration, float tdirection, float force);
		float checkDistance(float posX1, float posY1, float posX2, float posY2);
		float checkDirection(float x1, float y1, float x2, float y2);
		int duration;
		float direction;
		Move move;
		float vectorToDirection(sf::Vector2f vec);
		sf::Vector2f directionToVector(float angleDeg, float magnitude);
};