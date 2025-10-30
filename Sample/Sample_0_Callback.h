#pragma once
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
    class Sample_0_Callback : public SampleScene
    {
    private:
		std::weak_ptr<ISprite> m_target;
		std::weak_ptr<ITextLabel> m_statusLabel;
    public:
        Sample_0_Callback(std::unique_ptr<ICanvas>&& canvas);
        // IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
        void onStart() override;
		void onEnd() override;
    };
}//namespace

