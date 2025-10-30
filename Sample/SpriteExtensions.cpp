#include "pch.h"
#include "ICanvas.h"
#include "SpriteExtensions.h"

namespace LitMotionCpp::Sample
{
	void SpriteExtensions::ToX(float value, ISprite* target)
	{
		target->SetX(value);
	}

	void SpriteExtensions::ToColor(const Color4f& value, ISprite* target)
	{
		target->SetColor(value);
	}

}//namespace
