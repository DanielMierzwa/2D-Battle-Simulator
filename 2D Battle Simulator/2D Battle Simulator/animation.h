#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class Animation
{
private:
	sf::Texture spriteSheetTexture;
	std::vector<sf::Sprite> frames;
	int frameCount;
	float width;
	float height;
	// Helper function to load the sprite sheet and create frames
	void loadFrames(std::string spriteSheet, int framesCount);
public:
	Animation(std::string spriteSheet, int frames);
	sf::Sprite getFrame(int frameIndex, sf::Sprite sprite);
	int getFrameCount();

};

