#include "unitVector.h"
UnitVector::UnitVector(int tduration, float tdirection, float force){
	duration = tduration;
	direction = tdirection;
	float sinus=sin(direction * 3.14159 / 180);
	float cosinus = cos(direction * 3.14159 / 180);
	move.x = sinus *force;
	move.y = cosinus * force;
}
float UnitVector::checkDistance(float posX1, float posY1, float posX2, float posY2) {//sprawdza odleg³oœæ miêdzy punktami
	float a = abs(posX1 - posX2);
	float b = abs(posY1 - posY2);
	return sqrt(a * a + b * b);
}

float UnitVector::checkDirection(float posX1, float posY1, float posX2, float posY2) {//zwraca kierunek wektora z pierwszego punktu do drugiego
	//0* dó³
	//90* prawo
	//180* góra
	//270* lewo
	    float dx = abs(posX2 - posX1);
	    float dy = abs(posY2 - posY1);
		float sin = dy / checkDistance(posX1, posY1, posX2, posY2);

		if (dx == 0)
			if (posY1 < posY2)
				return 0;
			else
				return 180;
		if (dy == 0)
			if (posX1 < posX2)
				return 90;
			else
				return 270;

		float pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428;
		float direction = 0;
		if (posX1 < posX2 && posY1 < posY2) {
			direction = 90-(asin(sin) * 180 / pi);
		}
		if (posX1 > posX2 && posY1 > posY2) {
			direction = 270-(asin(sin) * 180 / pi);
		}
		if (posX1 < posX2 && posY1 > posY2) {
			direction = asin(sin) * 180 / pi - 270;
		}
		if (posX1 > posX2 && posY1 < posY2) {
			direction = asin(sin) * 180 / pi - 90;
		}
		direction = (float)((int)direction % 360);
		if (direction < 0) {
			direction += 360;
		}
		return direction;

	}
sf::Vector2f UnitVector::directionToVector(float angleDeg, float magnitude) {
	float rad = angleDeg * 3.14159265f / 180.f;
	return sf::Vector2f(std::cos(rad) * magnitude, std::sin(rad) * magnitude);
}

// Konwertuje wektor na kierunek (w stopniach)
float UnitVector::vectorToDirection(sf::Vector2f vec) {
	return std::atan2(vec.y, vec.x) * 180.f / 3.14159265f;
}
