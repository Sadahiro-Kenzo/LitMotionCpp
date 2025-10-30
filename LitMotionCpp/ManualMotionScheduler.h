#pragma once
#include "IMotionScheduler.h"
#include "ManualMotionDispatcher.h"

namespace LitMotionCpp
{
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	class ManualMotionScheduler:public IMotionScheduler<TValue,TOptions>
	{
	public:
		virtual MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData) override
		{
			return ManualMotionDispatcher::schedule<TValue,TOptions>(data, callbackData);
		}

		virtual double getTime() override
		{
			return ManualMotionDispatcher::getTime();
		}
	};
}//namespace

