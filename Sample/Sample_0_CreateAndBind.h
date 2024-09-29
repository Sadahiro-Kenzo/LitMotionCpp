#pragma once
#include <array>
#include "Interfaces.h"

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

		// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void onStart() override;
		void onUpdate(IInput&) override;
		void onDraw(IRenderer&) override;
		void onEnd() override;
	};
}//namespace

