#include "pch.h"
#include "Sample_0_CancelAndComplete.h"
#include "SpriteExtensions.h"
#include "TextLabelExtensions.h"
#include "ICanvas.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_CancelAndComplete::Sample_0_CancelAndComplete(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "6. Cancel & Complete",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		spec.text = "Press [space] to Cancel / Complete";
		spec.position.y = -180.0f;
		canvas.PushBackTextLabel(spec);

		auto target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, 1.5f);
		target->SetColor(Color4f{ 0.32f,0.69f,0.96f,1.0f });
		m_target1 = target;

		spec.text = "Cancel";
		spec.position = { -330.0f,150.0f };
		spec.pivot = { 0.0f,0.5f };
		spec.hAlign = HorizontalAlign::Left;
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, -1.0f);
		target->SetColor(Color4f{ 1.0f,0.0f,0.0f,1.0f });
		m_target2 = target;

		spec.text = "Complete";
		spec.position = { -330.0f,0.0f };
		canvas.PushBackTextLabel(spec);
	}

	void Sample_0_CancelAndComplete::onStart()
	{
		auto target = m_target1.lock();
		if (target)
		{
			m_handle1=LMotion::create(-5.0f, 5.0f, 2.0f)
				.withLoops(999)
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
		}

		target = m_target2.lock();
		if (target)
		{
			m_handle2 = LMotion::create(-5.0f, 5.0f, 2.0f)
				.withLoops(999)
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
		}

		SampleScene::onStart();
	}

	void Sample_0_CancelAndComplete::onUpdate(IInput& input)
	{
		if(SampleScene::IsPlaying() && input.pressedSpace())
		{
			if (m_handle1.isActive())
			{
				m_handle1.cancel();
			}
			if (m_handle2.isActive())
			{
				m_handle2.complete();
			}

			SampleScene::Complete();
			return;
		}

		SampleScene::onUpdate(input);
	}

	void Sample_0_CancelAndComplete::onEnd()
	{
	}
}// namespace
