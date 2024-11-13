#include "pch.h"
#include <format>
#include "Sample_0_CreateAndBind.h"
#include <LMotion.h>
#include "VecmathMotionAdapter.h"
#include "SpriteExtensions.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_CreateAndBind::Sample_0_CreateAndBind()
	{
		m_targetTransform.X = 116.0f;
		m_targetTransform.Y = 100.0f;
		m_targetTransform.Color =  Color4f{ 0.0f,1.0f,1.0f,1.0f };

		m_targetColor.X = 116.0f;
		m_targetColor.Y = 150.0f;
		m_targetColor.Color = Color4f{ 1.0f,0.0f,0.0f,1.0f };
	}

	void Sample_0_CreateAndBind::onStart()
	{
		LMotion::create(100.0f, 700.0f, 5.0f)
			.bindTo<Sprite>(SpriteExtensions::ToX, &m_targetTransform);

		Color4f red{ 1.0f,0.0f,0.0f,1.0f };
		Color4f blue{ 0.0f,0.0f,1.0f,1.0f };

		LMotion::create<Color4f>(red, blue, 5.0f)
			.bindTo<Sprite>(SpriteExtensions::ToColor, &m_targetColor);

		LMotion::create(0.0f, 10.0f, 5.0f)
			.bind([this](float x)
				{
					auto [end, _] = std::format_to_n(this->m_targetText.data(), this->m_targetText.size() - 1, "{}", x);
					*end = '\0';
				});
	}

	void Sample_0_CreateAndBind::onUpdate(IInput&)
	{
	}

	void Sample_0_CreateAndBind::onDraw(IRenderer& renderer)
	{
		renderer.drawText(200.0f, 0.0f, "Create & Bind");
		renderer.drawText(100.0f, 60.0f, "Position");
		renderer.drawSprite(m_targetTransform);
		renderer.drawText(100.0f, 110.0f, "Color");
		renderer.drawSprite(m_targetColor);
		renderer.drawText(100.0f, 170.0f, "Custom");
		renderer.drawText(100.0f, 200.0f, m_targetText.data());
	}

	void Sample_0_CreateAndBind::onEnd()
	{
	}
}// namespace
