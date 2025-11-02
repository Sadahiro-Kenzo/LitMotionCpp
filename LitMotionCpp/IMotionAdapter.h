#pragma once
#include <concepts>
#include "IMotionOptions.h"
namespace LitMotionCpp
{
	/**
	* @brief Provides basic information for evaluating motion.
	*/
	struct MotionEvaluationContext
	{
		float Progress;	//!< Progress (0-1)

		MotionEvaluationContext(float value):Progress(value){}
	};

	/**
	* @brief Implement this interface to define animating values of a particular type.
	* 
	* @tparam TValue : The type of value to animate
	* @tparam TOptions : The type of special parameters given to the motion entity
	*/
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	struct IMotionAdapter
	{
		/**
		* @brief Define the process to interpolate the values between two points.
		* 
		* @param [in] startValue : Start value
		* @param [in] endValue : End value
		* @param [in] options : Option value to specify
		* @param [in] context : Animation context
		* 
		* @return Current value
		*/
		virtual TValue evaluate(const TValue& startValue, const TValue& endValue, const TOptions& options, const MotionEvaluationContext& context) = 0;
	};
}//namespace