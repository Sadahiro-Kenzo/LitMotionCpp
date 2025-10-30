#pragma once
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
	class Sample_0_Loops:public SampleScene
	{
	private:
		std::weak_ptr<ISprite> m_target1;
		std::weak_ptr<ISprite> m_target2;
		std::weak_ptr<ISprite> m_target3;
	public:
		Sample_0_Loops(std::unique_ptr<ICanvas>&& canvas);

		// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void onStart() override;
		void onEnd() override;
	};
}//namespace

