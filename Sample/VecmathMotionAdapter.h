#pragma once
#include <LMotion.h>
#include <vecmath.h>

namespace LitMotionCpp
{
	template<> Color4f evaluate(const Color4f&, const Color4f&, const MotionEvaluationContext&);
}//namespace
