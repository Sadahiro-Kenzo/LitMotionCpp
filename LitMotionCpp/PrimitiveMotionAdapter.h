#pragma once
#include "MotionAdapter.h"

namespace LitMotionCpp
{
	template<> float evaluate(const float, const float, const MotionEvaluationContext&);
}//namespace