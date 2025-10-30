#include "pch.h"
#include "Sample_0_CreateAndBind.h"
#include <LMotion.h>
#include "VecmathMotionAdapter.h"
#include "SpriteExtensions.h"
#include "TextLabelExtensions.h"
#include "ICanvas.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_CreateAndBind::Sample_0_CreateAndBind(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text="1. Create & Bind",
			.position={0.0f,240.0f},
			.hAlign=HorizontalAlign::Center,
			.vAlign=VerticalAlign::Middle,
			.pivot={0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		spec.text = "Position";
		spec.position = { -330.0f,180.0f };
		spec.pivot = { 0.0f,0.5f };
		spec.hAlign = HorizontalAlign::Left;
		canvas.PushBackTextLabel(spec);

		auto targetTransform = canvas.PushBackSprite().lock();
		targetTransform->SetPosition(-5.0f, 2.0f);
		targetTransform->SetColor(Color4f{0.32f,0.69f,0.96f,1.0f});
		m_targetTransform = targetTransform;

		spec.text = "Color";
		spec.position = { -330.0f,30.0f };
		canvas.PushBackTextLabel(spec);

		auto targetColor = canvas.PushBackSprite().lock();
		targetColor->SetPosition(-5.0f, -0.5f);
		targetColor->SetColor(Color4f{ 1.0f,0.0f,0.0f,1.0f });
		m_targetColor = targetColor;

		spec.text = "Custom";
		spec.position = { -330.0f,-120.0f };
		canvas.PushBackTextLabel(spec);

		spec.text = "0.0";
		spec.position = { -330.0f,-180.0f };
		spec.color = { 0.7f,0.7f,0.7f,1.0f };
		m_targetText = canvas.PushBackTextLabel(spec);
	}

	void Sample_0_CreateAndBind::onStart()
	{
		MotionHandle handle;

		auto targetTransform = m_targetTransform.lock();
		if (targetTransform)
		{
			handle=LMotion::create(-5.0f, 5.0f, 5.0f)
				.bindWithState<ISprite>(targetTransform.get(),&SpriteExtensions::ToX);
			AddHandle(handle);
		}

		auto targetColor = m_targetColor.lock();
		if (targetColor)
		{
			Color4f red{ 1.0f,0.0f,0.0f,1.0f };
			Color4f blue{ 0.0f,0.0f,1.0f,1.0f };

			handle=LMotion::create<Color4f>(red, blue, 5.0f)
				.bindWithState<ISprite>(targetColor.get(),&SpriteExtensions::ToColor);
			AddHandle(handle);
		}

		auto targetText = m_targetText.lock();
		if (targetText)
		{
			handle=LMotion::create(0.0f, 10.0f, 5.0f)
				.bindWithState<ITextLabel>(targetText.get(), &TextLabelExtensions::ToText);
			AddHandle(handle);
		}

		SampleScene::onStart();
	}

	void Sample_0_CreateAndBind::onEnd()
	{
	}
}// namespace
