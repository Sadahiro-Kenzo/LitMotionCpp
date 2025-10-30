#pragma once
#include "MotionHandle.h"
#include "MotionData.h"
#include "MotionCallbackData.h"

namespace LitMotionCpp
{
	/**
	* @brief Provides the function to schedule the execution of a motion.
	*/
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	class IMotionScheduler
	{
	public:
		IMotionScheduler() = default;
		virtual ~IMotionScheduler() = default;

		virtual MotionHandle schedule(const MotionData<TValue,TOptions>&,const MotionCallbackData&) = 0;

		virtual double getTime() = 0;
	};

	enum class MotionTimeKind
	{
		Time,
		UnscaledTime,
		Realtime,
	};

}//namespace