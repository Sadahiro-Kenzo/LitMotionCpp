#include "pch.h"
#include "Sample_0_Callback.h"
#include <LMotion.h>
#include "SpriteExtensions.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_Callback::Sample_0_Callback(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "5. Callback",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		auto target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, 0.0f);
		target->SetColor(Color4f{ 0.32f,0.69f,0.96f,1.0f });
		m_target = target;

		spec.text = "Playing...";
		spec.position.y=-60.0f;
		m_statusLabel=canvas.PushBackTextLabel(spec);
	}

	void Sample_0_Callback::onStart()
	{
		auto target = m_target.lock();
		if (target)
		{
			LMotion::create(-5.0f, 5.0f, 2.0f)
				.withEase(Ease::InOutSine)
				.withOnComplete([this]()
					{
						auto label = m_statusLabel.lock();
						if (label)
						{
							label->SetText("Complete!");
						}
						Complete();
					})
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
		}

		auto label = m_statusLabel.lock();
		if (label)
		{
			label->SetText("Playing...");
		}

		SampleScene::onStart();
	}

	void Sample_0_Callback::onEnd()
	{
	}
}// namespace
