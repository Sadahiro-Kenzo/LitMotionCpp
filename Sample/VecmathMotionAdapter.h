#pragma once
#include <LMotion.h>
#include <vecmath.h>

namespace LitMotionCpp
{
	template<typename TTuple,typename TValue>
		requires std::derived_from<TTuple, Tuple4<TValue>>
	struct Tuple4MotionAdapter : public IMotionAdapter<TTuple,NoOptions>
	{
		TTuple evaluate(const TTuple& startValue, const TTuple& endValue,const NoOptions& /*options*/, const MotionEvaluationContext& context) override
		{
			auto result{ startValue };
			result.interpolate(endValue, context.Progress);
			return result;
		}
	};

	typedef Tuple4MotionAdapter<Color4f, float> Color4fMotionAdapter;

}//namespace
