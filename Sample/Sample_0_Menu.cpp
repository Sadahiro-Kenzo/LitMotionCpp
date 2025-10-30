#include "pch.h"
#include "Sample_0_Menu.h"
#include "TextLabelExtensions.h"
#include "Message.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Sample_0_Menu::Sample_0_Menu(std::unique_ptr<ICanvas>&& initialCanvas)
		:Scene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = "1. Create & Bind",
			.color = {1.0f,0.0f,0.0f,1.0f},
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));

		spec.text = "2. Easing";
		spec.color = { 0.0f,0.0f,0.0f,1.0f };
		spec.position.y -= 60.0f;
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));

		spec.text = "3. Loops";
		spec.position.y -= 60.0f;
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));

		spec.text = "4. Delay";
		spec.position.y -= 60.0f;
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));

		spec.text = "5. Callback";
		spec.position.y -= 60.0f;
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));

		spec.text = "6. Cancel & Complete";
		spec.position.y -= 60.0f;
		m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));
	}

	void Sample_0_Menu::onStart()
	{
	}

	void Sample_0_Menu::onUpdate(IInput& input)
	{
		if(input.pressedUp())
		{
			m_menuItems[m_currentIndex].lock()->SetColor(Color4f{ 0.0f,0.0f,0.0f,1.0f });
			m_currentIndex = (m_currentIndex - 1 + static_cast<int>(m_menuItems.size())) % m_menuItems.size();
			m_menuItems[m_currentIndex].lock()->SetColor(Color4f{ 1.0f,0.0f,0.0f,1.0f });
		}
		else if(input.pressedDown())
		{
			m_menuItems[m_currentIndex].lock()->SetColor(Color4f{ 0.0f,0.0f,0.0f,1.0f });
			m_currentIndex = (m_currentIndex + 1) % m_menuItems.size();
			m_menuItems[m_currentIndex].lock()->SetColor(Color4f{ 1.0f,0.0f,0.0f,1.0f });
		}
		else if (input.pressedSpace())
		{
			Message::Post(Message::Type::Select, m_currentIndex);
		}
	}

	void Sample_0_Menu::onEnd()
	{
	}
}// namespace
