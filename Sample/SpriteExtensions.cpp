#include "pch.h"
#include "Sprite.h"
#include "SpriteExtensions.h"
#include "DirectXMotionAdapter.h"

namespace LitMotionCpp::Sample
{
	MotionBuilder<float>::PropertyBinder<Sprite> SpriteExtensions::ToX
		= [](MotionBuilder<float>* builder, Sprite* target)
		{
			return builder->bindWithState<Sprite>(target, [](float value, Sprite* t) {t->X = value; });
		};

	MotionBuilder<DirectX::XMFLOAT4>::PropertyBinder<Sprite> SpriteExtensions::ToColor
		= [](MotionBuilder<DirectX::XMFLOAT4>* builder, Sprite* target)
		{
			return builder->bindWithState<Sprite>(target, [](DirectX::XMFLOAT4& value, Sprite* t) {t->Color = value; });
		};

}//namespace
