#include "pch.h"
#include "Sample_0_Loops.h"
#include <LMotion.h>
#include "VecmathMotionAdapter.h"
#include "SpriteExtensions.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_Loops::Sample_0_Loops(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		const Color4f color{ 0.32f,0.69f,0.96f,1.0f };

		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "3. Loops",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		spec.text = "LoopType.Restart";
		spec.position = { -330.0f,180.0f };
		spec.pivot = { 0.0f,0.5f };
		spec.hAlign = HorizontalAlign::Left;
		canvas.PushBackTextLabel(spec);

		auto target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, 2.0f);
		target->SetColor(color);
		m_target1 = target;

		spec.text = "LoopType.Yoyo";
		spec.position = { -330.0f,30.0f };
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, -0.5f);
		target->SetColor(color);
		m_target2 = target;

		spec.text = "LoopType.Incremental";
		spec.position = { -330.0f,-120.0f };
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, -3.0f);
		target->SetColor(color);
		m_target3 = target;
	}

	void Sample_0_Loops::onStart()
	{
		MotionHandle handle;

		auto target = m_target1.lock();
		if (target)
		{
			handle=LMotion::create(-5.0f, 5.0f, 1.5f)
				.withEase(Ease::OutSine)
				.withLoops(5, LoopType::Restart)
				.bind<ISprite>(target.get(), SpriteExtensions::ToX);
			AddHandle(handle);
		}

		target = m_target2.lock();
		if (target)
		{
			handle=LMotion::create(-5.0f, 5.0f, 1.5f)
				.withEase(Ease::OutSine)
				.withLoops(5, LoopType::Yoyo)
				.bind<ISprite>(target.get(), SpriteExtensions::ToX);
			AddHandle(handle);
		}

		target = m_target3.lock();
		if (target)
		{
			handle=LMotion::create(-5.0f, -3.0f, 1.5f)
				.withEase(Ease::OutSine)
				.withLoops(5, LoopType::Incremental)
				.bind<ISprite>(target.get(), SpriteExtensions::ToX);
			AddHandle(handle);
		}

		SampleScene::onStart();
	}

	void Sample_0_Loops::onEnd()
	{
	}
}// namespace
