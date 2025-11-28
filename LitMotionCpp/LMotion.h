#pragma once
// If using exception, uncomment this line:
//#define LIT_MOTION_CPP_ENABLE_EXCEPTION

#include "MotionBuilder.h"
#include "MotionScheduler.h"
#include "PrimitiveMotionAdapter.h"
#include "NoOptions.h"
#include "IntegerOptions.h"

namespace LitMotionCpp
{
	/**
	* @brief The main class of the LitMotionCpp library that creates and configures motion.
	*/
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
		static auto create(float from, float to, float duration)
		{
			return create<float,NoOptions,FloatMotionAdapter<float>>(from, to, duration);
		}

		/**
		* @brief Create a builder for building motion.
		*
		* @param [in] from : Start value
		* @param [in] to : End value
		* @param [in] duration : Duration
		*
		* @return Created motion builder
		*/
		static auto create(double from, double to, float duration)
		{
			return create<double, NoOptions, FloatMotionAdapter<double>>(from, to, duration);
		}

		/**
		* @brief Create a builder for building motion.
		*
		* @param [in] from : Start value
		* @param [in] to : End value
		* @param [in] duration : Duration
		*
		* @return Created motion builder
		*/
		static auto create(int from, int to, float duration)
		{
			return create<int, IntegerOptions, IntegerMotionAdapter<int>>(from, to, duration);
		}

		/**
		* @brief Create a builder for building motion.
		* 
		* @tparam TValue : The type of value to animate
		* @tparam TOptions : The type of special parameters given to the motion entity
		* @tparam TAdapter : The type of adapter that support value animation
		* @param [in] from : Start value
		* @param [in] to : End value
		* @param [in] duration : Duration
		* 
		* @return Created motion builder
		*/
		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static MotionBuilder<TValue,TOptions,TAdapter> create(const TValue& from, const TValue& to, float duration)
		{
			return MotionBuilder<TValue,TOptions,TAdapter>(from, to, duration);
		}
	};
}//namespace

