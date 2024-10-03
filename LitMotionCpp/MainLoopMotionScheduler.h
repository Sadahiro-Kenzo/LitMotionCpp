#pragma once
#include "IMotionScheduler.h"
#include "MotionDispatcher.h"

namespace LitMotionCpp
{
	template<typename TValue>
	class MainLoopMotionScheduler :public IMotionScheduler<TValue>
	{
	private:
		int m_loopTiming;
		MotionTimeKind m_timeKind;
	public:
		MainLoopMotionScheduler(MotionTimeKind timeKind)
			:m_loopTiming(0)
			,m_timeKind(timeKind)
		{}

		virtual MotionHandle schedule(const MotionData<TValue>& data, const MotionCallbackData& callbackData) override
		{
			return MotionDispatcher::schedule<TValue>(data, callbackData,m_loopTiming);
		}

		virtual double getTime() override
		{
			return MotionDispatcher::getTime();
		}

		int getLoopTiming() { return m_loopTiming; }
		MotionTimeKind getTimeKind() { return m_timeKind; }
	};
}//namespace