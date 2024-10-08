#pragma once
#include <array>
#include "Interfaces.h"
#include "Sprite.h"

namespace LitMotionCpp::Sample
{
	class Sample_0_CreateAndBind:public IScene
	{
	private:
		Sprite m_targetTransform;
		Sprite m_targetColor;
		std::array<char, 16> m_targetText;
	public:
		Sample_0_CreateAndBind();

		// IScene を介して継承されました
		void onStart() override;
		void onUpdate(IInput&) override;
		void onDraw(IRenderer&) override;
		void onEnd() override;
	};
}//namespace

