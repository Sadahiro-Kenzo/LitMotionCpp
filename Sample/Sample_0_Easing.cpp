#include "pch.h"
#include "NotifyMemoryLeak.h"
#include "Sample_0_Easing.h"
#include <LMotion.h>
#include <AnimationCurve.h>
#include "VecmathMotionAdapter.h"
#include "SpriteExtensions.h"

using namespace LitMotionCpp;
using namespace DirectX;

const Keyframe keyframes[] = {
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.32483720779418948f, 0.9665907025337219f, 0.08513136953115463f, 0.08513136953115463f},
	{0.6239457130432129f, -0.027526676654815675f, 0.59372878074646f, 0.59372878074646f},
	{0.6815565824508667f, 1.0051050186157227f, -0.017104098573327066f, -0.017104098573327066f},
	{0.8800565600395203f, -0.01362311840057373f, -0.00554767856374383f, -0.00554767856374383f},
	{1.0f, 1.0f, 0.0f, 0.0f}
};

namespace LitMotionCpp::Sample
{
	Sample_0_Easing::Sample_0_Easing(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		const Color4f color{ 0.32f,0.69f,0.96f,1.0f };

		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "2. Easing",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		spec.text = "Linear";
		spec.position = { -330.0f,180.0f };
		spec.pivot = { 0.0f,0.5f };
		spec.hAlign = HorizontalAlign::Left;
		canvas.PushBackTextLabel(spec);

		auto target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, 2.0f);
		target->SetColor(color);
		m_target1 = target;

		spec.text = "In Quad";
		spec.position = { -330.0f,60.0f };
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, 0.0f);
		target->SetColor(color);
		m_target2 = target;

		spec.text = "Out Bounce";
		spec.position = { -330.0f,-60.0f };
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, -2.0f);
		target->SetColor(color);
		m_target3 = target;

		spec.text = "AnimationCurve";
		spec.position = { -330.0f,-180.0f };
		canvas.PushBackTextLabel(spec);

		target = canvas.PushBackSprite().lock();
		target->SetPosition(-5.0f, -4.0f);
		target->SetColor(color);
		m_target4 = target;
	}

	void Sample_0_Easing::onStart()
	{
		MotionHandle handle;

		auto target = m_target1.lock();
		if (target)
		{
			handle=LMotion::create(-5.0f, 5.0f, 3.0f)
				.withEase(Ease::Linear)
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
			AddHandle(handle);
		}

		target = m_target2.lock();
		if (target)
		{
			handle = LMotion::create(-5.0f, 5.0f, 3.0f)
				.withEase(Ease::InQuad)
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
			AddHandle(handle);
		}

		target = m_target3.lock();
		if (target)
		{
			handle = LMotion::create(-5.0f, 5.0f, 3.0f)
				.withEase(Ease::OutBounce)
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
			AddHandle(handle);
		}

		target = m_target4.lock();
		if (target)
		{
			handle = LMotion::create(-5.0f, 5.0f, 3.0f)
				.withCustomCurve(std::begin(keyframes), std::end(keyframes))
				.bindWithState<ISprite>(target.get(), &SpriteExtensions::ToX);
			AddHandle(handle);
		}

		SampleScene::onStart();
	}

	void Sample_0_Easing::onEnd()
	{
	}
}// namespace
