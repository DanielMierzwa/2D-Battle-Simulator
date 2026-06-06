#pragma once
#include "animation.h"
#include <iostream>
class AnimationSource
{
public:
	AnimationSource();
	Animation attackAnim[4][2] = {
			{Animation("graphics/good/w³ócznik_atak.png", 9),//zieloni
	Animation("graphics/bad/w³ócznik_atak.png", 9)},//fioletowi

	{Animation("graphics/good/lucznik_atak.png", 7),
	Animation("graphics/bad/lucznik_atak.png", 7)},

	{Animation("graphics/good/jezdziec_atak.png", 5),
	Animation("graphics/bad/jezdziec_atak.png", 5) },

	{Animation("graphics/good/kusznik_atak.png", 7),
	Animation("graphics/bad/kusznik_atak.png", 7)}
	};
	Animation walkAnim[4][2] =
	{
	{ Animation("graphics/good/w³ócznik_chodzenie.png", 2),
	Animation("graphics/bad/w³ócznik_chodzenie.png", 2)},

	{ Animation("graphics/good/lucznik_chodzenie.png", 2),
	Animation("graphics/bad/lucznik_chodzenie.png", 2)},

	{ Animation("graphics/good/jezdziec_jazda.png", 3),
	Animation("graphics/bad/jezdziec_jazda.png", 3) },

	{ Animation("graphics/good/kusznik_chodzenie.png", 2),
	Animation("graphics/bad/kusznik_chodzenie.png", 2)}
	};


};

