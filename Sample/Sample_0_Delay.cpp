#include "pch.h"
#include "Sample_0_Delay.h"
#include <LMotion.h>
#include "VecmathMotionAdapter.h"
#include "SpriteExtensions.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_Delay::Sample_0_Delay(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "4. Delay",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		for(auto i=0; i<4; i++)
		{
			m_targets.push_back(canvas.PushBackSprite());
			auto target = m_targets.back().lock();
			target->SetPosition(-5.0f, 2.0f-1.5f*i);
			target->SetColor(Color4f{ 0.32f,0.69f,0.96f,1.0f });
		}
	}

	void Sample_0_Delay::onStart()
	{
		for(auto i=0; i<4; i++)
		{
			auto target = m_targets[i].lock();
			if (target)
			{
				auto handle = LMotion::create(-5.0f, 5.0f, 2.0f)
					.withEase(Ease::InOutSine)
					.withDelay(i * 0.2f)
					.withBindOnSchedule()
					.bind<ISprite>(target.get(), SpriteExtensions::ToX);
				AddHandle(handle);
			}
		}

		SampleScene::onStart();
	}

	void Sample_0_Delay::onEnd()
	{
	}
}// namespace
