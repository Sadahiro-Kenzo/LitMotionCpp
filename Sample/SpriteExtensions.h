#pragma once
#include <functional>
#include <LMotion.h>
#include "DirectXMath.h"

namespace LitMotionCpp::Sample
{
	struct Sprite;

	/**
	* @brief Extension functions bind Sprite property.
	*/
	struct SpriteExtensions
	{
		static MotionBuilder<float>::PropertyBinder<Sprite> ToX;
		static MotionBuilder<DirectX::XMFLOAT4>::PropertyBinder<Sprite> ToColor;
	};
}//namespace
