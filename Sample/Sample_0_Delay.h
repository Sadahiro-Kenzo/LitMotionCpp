#pragma once
#include <vector>
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
    class Sample_0_Delay : public SampleScene
    {
    private:
			std::vector<std::weak_ptr<ISprite>> m_targets;
	public:
		Sample_0_Delay(std::unique_ptr<ICanvas>&& canvas);

		// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void onStart() override;
		void onEnd() override;
    };

}//namespace


