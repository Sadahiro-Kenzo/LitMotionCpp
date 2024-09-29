#include "pch.h"
#include <cmath>
#include "PrimitiveMotionAdapter.h"

namespace LitMotionCpp
{
	template<>
	float evaluate<float>(const float& startValue, const float& endValue, const MotionEvaluationContext& context)
	{
		return std::lerp(startValue, endValue, context.Progress);
	}
}//namespace