#include "animation.h"
using namespace sf;
Animation::Animation(std::string spriteSheet, int framesCount) : frameCount(framesCount) {
    if (!spriteSheetTexture.loadFromFile(spriteSheet)) {
        std::cerr << "Nie uda³o siê wczytaæ spritesheeta!" << std::endl;
    }
 width = spriteSheetTexture.getSize().x / frameCount;
 height = spriteSheetTexture.getSize().y;
	for (int i = 0; i < frameCount; ++i) {
		Sprite sprite;
		sprite.setTexture(spriteSheetTexture);
		sprite.setTextureRect(IntRect(i * width, 0, width, height));
		frames.push_back(sprite);
	}
}
Sprite Animation::getFrame(int frameIndex, Sprite sprite) {
	if (frameIndex < 0 || frameIndex >= frameCount) {
		std::cerr << "Indeks klatki poza zakresem!" << std::endl;
	}
	sprite.setTexture(spriteSheetTexture);
	sprite.setTextureRect(IntRect(frameIndex * width, 0, width, height));
	return sprite;
}

int Animation::getFrameCount() {
	return frameCount;
}