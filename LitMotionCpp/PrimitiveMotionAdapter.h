#pragma once
#include <cmath>
#include "IMotionAdapter.h"
#include "NoOptions.h"
#include "IntegerOptions.h"

namespace LitMotionCpp
{
	/**
	* @brief Implementation of IMotionAdapter for floating point types.
	*/
	template<std::floating_point TValue>
	struct FloatMotionAdapter :public IMotionAdapter<TValue, NoOptions>
	{
		TValue evaluate(const TValue& startValue, const TValue& endValue, const NoOptions& /*options*/, const MotionEvaluationContext& context) override
		{
			return std::lerp(startValue, endValue, context.Progress);
		}
	};

	/*
	* @brief Implementation of IMotionAdapter for integer types.
	*/
	template<std::integral TValue>
	struct IntegerMotionAdapter :public IMotionAdapter<TValue, IntegerOptions>
	{
		TValue evaluate(const TValue& startValue, const TValue& endValue, const IntegerOptions& options, const MotionEvaluationContext& context) override
		{
			auto lerped = std::lerp(static_cast<double>(startValue), static_cast<double>(endValue), context.Progress);
			switch (options.roundingMode)
			{
			case RoundingMode::AwayFromZero:	return static_cast<TValue>(lerped >= 0 ? std::ceil(lerped) : std::floor(lerped));
			case RoundingMode::ToZero:	return static_cast<TValue>(std::trunc(lerped));
			case RoundingMode::ToPositiveInfinity:	return static_cast<TValue>(std::ceil(lerped));
			case RoundingMode::ToNegativeInfinity:	return static_cast<TValue>(std::floor(lerped));
			default:return static_cast<TValue>(std::round(lerped));
			}
		}
	};
}//namespace