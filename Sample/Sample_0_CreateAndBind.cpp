#include "pch.h"
#include "Sample_0_CreateAndBind.h"
#include <LMotion.h>
#include "DirectXMotionAdapter.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_CreateAndBind::Sample_0_CreateAndBind()
	{
		m_targetTransform.X = 116.0f;
		m_targetTransform.Y = 100.0f;
		m_targetTransform.Color =  XMFLOAT4{ 0.0f,1.0f,1.0f,1.0f };

		m_targetColor.X = 116.0f;
		m_targetColor.Y = 150.0f;
		m_targetColor.Color = XMFLOAT4{ 1.0f,0.0f,0.0f,1.0f };
	}

	void Sample_0_CreateAndBind::onStart()
	{
		LMotion::Create(100.0f, 700.0f, 5.0f)
			.bindWithState<Sprite>(&m_targetTransform,[](float x,Sprite* target) {target->X = x+16.0f; });

		XMFLOAT4 red{ 1.0f,0.0f,0.0f,1.0f };
		XMFLOAT4 blue{ 0.0f,0.0f,1.0f,1.0f };

		LMotion::Create<XMFLOAT4>(red, blue, 5.0f)
			.bindWithState<Sprite>(&m_targetColor,[](XMFLOAT4& value,Sprite* target) {target->Color = value; });

		LMotion::Create(0.0f, 10.0f, 5.0f)
			.bind([this](float x)
				{
					sprintf(this->m_targetText.data(), "%f", x);
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
