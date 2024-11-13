#include "pch.h"
#include "VecmathMotionAdapter.h"

namespace LitMotionCpp
{
	template<>
	Color4f evaluate(const Color4f& startValue, const Color4f& endValue, const MotionEvaluationContext& context)
	{
		auto result{ startValue };
		result.interpolate(endValue, context.Progress);
		return result;
	}
}//namespace