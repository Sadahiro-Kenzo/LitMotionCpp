#include "pch.h"
#include <algorithm>
#include "SampleScene.h"
#include "Message.h"

namespace LitMotionCpp::Sample
{
	void SampleScene::onStart()
	{
		m_state = State::Playing;
	}

	void SampleScene::onUpdate(IInput& input)
	{
		switch (m_state)
		{
		case LitMotionCpp::Sample::SampleScene::State::Ready:
			break;
		case LitMotionCpp::Sample::SampleScene::State::Playing:
		{
			if (!m_handles.empty())
			{
				if (std::all_of(m_handles.begin(), m_handles.end(), [](const MotionHandle& handle) { return !handle.isActive(); }))
				{
					Complete();
				}
			}
			break;
		}
		case LitMotionCpp::Sample::SampleScene::State::Completed:
		{
			if (input.pressedEscape())
			{
				Message::Post(Message::Type::Close, 0);
				return;
			}
			if (input.pressedSpace())
			{
				onStart();
			}

			break;
		}
		default:
			break;
		}

	}

	void SampleScene::Complete()
	{
		if (m_state == State::Playing)
		{
			m_handles.clear();
			m_state = State::Completed;
		}
	}
}// namespace