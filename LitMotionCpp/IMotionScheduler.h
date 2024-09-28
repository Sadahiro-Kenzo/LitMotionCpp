#pragma once
#include "MotionHandle.h"
#include "MotionData.h"
#include "MotionCallbackData.h"

namespace LitMotionCpp
{
	/**
	* @brief Provides the function to schedule the execution of a motion.
	*/
	template<typename TValue>
	class IMotionScheduler
	{
	public:
		IMotionScheduler() = default;
		virtual ~IMotionScheduler() = default;

		virtual MotionHandle schedule(const MotionData<TValue>&,const MotionCallbackData<TValue>&) = 0;

		virtual double getTime() = 0;
	};

	enum class MotionTimeKind
	{
		Time,
		UnscaledTime,
		Realtime,
	};

}//namespace