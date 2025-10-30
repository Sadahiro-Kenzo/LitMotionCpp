#pragma once
#include <vecmath.h>

namespace LitMotionCpp::Sample
{
	class ISprite;

	/**
	* @brief Extension functions bind Sprite property.
	*/
	struct SpriteExtensions
	{
		static void ToX(float, ISprite*);
		static void ToColor(const Color4f&, ISprite*);
	};
}//namespace
