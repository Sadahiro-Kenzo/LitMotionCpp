#include "pch.h"
#include <cassert>
#include "ManualMotionDispatcher.h"

namespace LitMotionCpp
{
	FastListCore<std::weak_ptr<IUpdateRunner>> ManualMotionDispatcher::m_updateRunners;

	static double s_time = 0.0;
	double ManualMotionDispatcher::getTime()
	{
		return s_time;
	}

	void ManualMotionDispatcher::setTime(double value)
	{
		s_time = value;
	}

	void ManualMotionDispatcher::update(double deltaTime)
	{
		assert(deltaTime >= 0.0);
		s_time += deltaTime;
		update();
	}

	void ManualMotionDispatcher::update()
	{
		auto span = m_updateRunners.asSpan();
		for (auto& i : span)
		{
			i.lock()->update(s_time, s_time, s_time);
		}
	}

	void ManualMotionDispatcher::reset()
	{
		auto span = m_updateRunners.asSpan();
		for (auto& i : span)
		{
			i.lock()->reset();
		}
	}

}//namespace
