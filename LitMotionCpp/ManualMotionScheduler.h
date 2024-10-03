#pragma once
#include "IMotionScheduler.h"
#include "ManualMotionDispatcher.h"

namespace LitMotionCpp
{
	template<typename TValue>
	class ManualMotionScheduler:public IMotionScheduler<TValue>
	{
	public:
		virtual MotionHandle schedule(const MotionData<TValue>& data, const MotionCallbackData& callbackData) override
		{
			return ManualMotionDispatcher::schedule<TValue>(data, callbackData);
		}

		virtual double getTime() override
		{
			return ManualMotionDispatcher::getTime();
		}
	};
}//namespace

