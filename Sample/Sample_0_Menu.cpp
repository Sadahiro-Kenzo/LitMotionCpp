#include "pch.h"
#include <array>
#include "Sample_0_Menu.h"
#include "TextLabelExtensions.h"
#include "Message.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	Color4f textColor = { 0.0f,0.0f,0.0f,1.0f };
	Color4f selectedColor = { 1.0f,0.0f,0.0f,1.0f };

	const char* menuItems[] = {
		"Create & Bind",
		"Easing",
		"Loops",
		"Delay",
		"Callback",
		"Cancel & Complete",
		"Benchmark"
	};

	Sample_0_Menu::Sample_0_Menu(std::unique_ptr<ICanvas>&& initialCanvas)
		:Scene(std::move(initialCanvas))
	{
		auto& canvas = getCanvas();

		TextLabelSpec spec{
			.text = nullptr,
			.color = textColor,
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};

		std::array<char, 32> buffer;
		int index = 1;
		for(auto name: menuItems)
		{
			sprintf_s(buffer.data(),buffer.size(), "%d. %s",index, name);
			spec.text = buffer.data();
			m_menuItems.emplace_back(canvas.PushBackTextLabel(spec));
			spec.position.y -= 60.0f;
			++index;
		}

		m_menuItems[m_currentIndex].lock()->SetColor(selectedColor);
	}

	void Sample_0_Menu::onStart()
	{
	}

	void Sample_0_Menu::onUpdate(IInput& input)
	{
		if(input.pressedUp())
		{
			m_menuItems[m_currentIndex].lock()->SetColor(textColor);
			m_currentIndex = (m_currentIndex - 1 + static_cast<int>(m_menuItems.size())) % m_menuItems.size();
			m_menuItems[m_currentIndex].lock()->SetColor(selectedColor);
		}
		else if(input.pressedDown())
		{
			m_menuItems[m_currentIndex].lock()->SetColor(textColor);
			m_currentIndex = (m_currentIndex + 1) % m_menuItems.size();
			m_menuItems[m_currentIndex].lock()->SetColor(selectedColor);
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
