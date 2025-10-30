#pragma once
// If using exception, uncomment this line:
//#define LIT_MOTION_CPP_ENABLE_EXCEPTION

#include "MotionBuilder.h"
#include "MotionScheduler.h"
#include "MainLoopMotionScheduler.h"
#include "ManualMotionDispatcher.h"
#include "PrimitiveMotionAdapter.h"
#include "NoOptions.h"
#include "IntegerOptions.h"

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
		static MotionBuilder<float,NoOptions> create(float from, float to, float duration)
		{
			return create<float,NoOptions>(from, to, duration);
		}

		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static MotionBuilder<TValue,TOptions> create(const TValue& from, const TValue& to, float duration)
		{
			return MotionBuilder<TValue,TOptions>(from, to, duration);
		}
	};
}//namespace

