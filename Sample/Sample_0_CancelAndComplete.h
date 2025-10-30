#pragma once
#include <LMotion.h>
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
    class Sample_0_CancelAndComplete : public SampleScene
    {
	private:
		std::weak_ptr<ISprite> m_target1;
		std::weak_ptr<ISprite> m_target2;

		MotionHandle m_handle1;
		MotionHandle m_handle2;
	public:
		Sample_0_CancelAndComplete(std::unique_ptr<ICanvas>&& canvas);

		// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		void onStart() override;
		void onUpdate(IInput&) override;
		void onEnd() override;
	};

}//namespace

