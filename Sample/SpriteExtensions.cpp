#include "pch.h"
#include "Sprite.h"
#include "SpriteExtensions.h"
#include "VecmathMotionAdapter.h"

namespace LitMotionCpp::Sample
{
	MotionBuilder<float>::PropertyBinder<Sprite> SpriteExtensions::ToX
		= [](MotionBuilder<float>* builder, Sprite* target)
		{
			return builder->bindWithState<Sprite>(target, [](float value, Sprite* t) {t->X = value; });
		};

	MotionBuilder<Color4f>::PropertyBinder<Sprite> SpriteExtensions::ToColor
		= [](MotionBuilder<Color4f>* builder, Sprite* target)
		{
			return builder->bindWithState<Sprite>(target, [](Color4f& value, Sprite* t) {t->Color = value; });
		};

}//namespace
