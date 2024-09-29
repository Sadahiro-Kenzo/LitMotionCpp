#include "pch.h"
#include "MotionDispatcher.h"

namespace LitMotionCpp
{
	std::map<int, FastListCore<std::weak_ptr<IUpdateRunner>>> MotionDispatcher::fastUpdateRunners;

	static double s_time = 0.0;
	double MotionDispatcher::getTime()
	{
		return s_time;
	}

	void MotionDispatcher::setTime(double value)
	{
		s_time = value;
	}

	void MotionDispatcher::update()
	{
		if (fastUpdateRunners.empty())
		{
			return;
		}

		auto span = fastUpdateRunners[0].asSpan();
		for (auto& i : span)
		{
			i.lock()->update(s_time, s_time, s_time);
		}
	}

	void MotionDispatcher::clear()
	{
		for (auto& runners : fastUpdateRunners)
		{
			auto span = runners.second.asSpan();
			for (auto& i : span)
			{
				i.lock()->reset();
			}
		}
	}

}//namespace