#pragma once
#include "IMotionScheduler.h"
#include "ManualMotionDispatcher.h"

namespace LitMotionCpp
{
	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	class ManualMotionScheduler:public IMotionScheduler<TValue,TOptions,TAdapter>
	{
	public:
		virtual MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData) override
		{
			return ManualMotionDispatcher::schedule<TValue,TOptions,TAdapter>(data, callbackData);
		}

		virtual double getTime() override
		{
			return ManualMotionDispatcher::getTime();
		}
	};
}//namespace

