#pragma once
#include <functional>
#include <LMotion.h>
#include <vecmath.h>

namespace LitMotionCpp::Sample
{
	struct Sprite;

	/**
	* @brief Extension functions bind Sprite property.
	*/
	struct SpriteExtensions
	{
		static MotionBuilder<float>::PropertyBinder<Sprite> ToX;
		static MotionBuilder<Color4f>::PropertyBinder<Sprite> ToColor;
	};
}//namespace
