#pragma once
#include "DirectXMath.h"

namespace LitMotionCpp::Sample
{
	/**
	* @brief GameObject
	*/
	struct Sprite
	{
		float X = 0.0f;
		float Y = 0.0f;
		DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4{};
	};
}//namespace
