#pragma once
// If using exception, uncomment this line:
//#define LIT_MOTION_CPP_ENABLE_EXCEPTION

#include "MotionBuilder.h"
#include "MotionScheduler.h"
#include "MainLoopMotionScheduler.h"
#include "ManualMotionDispatcher.h"
#include "PrimitiveMotionAdapter.h"

namespace LitMotionCpp
{
	class LMotion
	{
	public:
		/**
		* @brief Create a builder for building motion.
		* 
		* @param [in] from : Start value
		* @param [in] to : End value
		* @param [in] duration : Duration
		* 
		* @return Created motion builder
		*/
		static MotionBuilder<float> Create(float from, float to, float duration)
		{
			return Create<float>(from, to, duration);
		}

		template<typename TValue>
		static MotionBuilder<TValue> Create(const TValue& from, const TValue& to, float duration)
		{
			return MotionBuilder<TValue>(from, to, duration);
		}
	};
}//namespace

