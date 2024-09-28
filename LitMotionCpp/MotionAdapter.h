#pragma once
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
	* @brief Implement this template to define animating values of a particular type.
	*/
	template<typename TValue>
	TValue evaluate(const TValue startValue,const TValue endValue, const MotionEvaluationContext& context)
	{
		return TValue{};
	}
}//namespace