#pragma once
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
	class Sample_0_CreateAndBind:public SampleScene
	{
	private:
		std::weak_ptr<ISprite> m_targetTransform;
		std::weak_ptr<ISprite> m_targetColor;
		std::weak_ptr<ITextLabel> m_targetText;
	public:
		Sample_0_CreateAndBind(std::unique_ptr<ICanvas>&& canvas);

		// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void onStart() override;
		void onEnd() override;
	};
}//namespace

