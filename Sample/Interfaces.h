#pragma once
#include "DirectXMath.h"

namespace LitMotionCpp::Sample
{
	struct Sprite;

	class IInput
	{
	public:
		virtual bool pressedUp() = 0;
		virtual bool pressedDown()=0;
		virtual bool pressedSpace() = 0;
	};

	class IRenderer
	{
	public:
		virtual void drawSprite(Sprite& sprite) = 0;
		virtual void drawText(float x, float y, const char* text) = 0;
	};

	class IScene
	{
	public:
		IScene() = default;
		virtual ~IScene() = default;
		virtual void onStart() = 0;
		virtual void onUpdate(IInput&) = 0;
		virtual void onDraw(IRenderer&) = 0;
		virtual void onEnd() = 0;
	};
}//namespace
