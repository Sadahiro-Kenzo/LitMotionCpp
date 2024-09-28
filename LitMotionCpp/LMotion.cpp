#include "pch.h"
#include "LMotion.h"

namespace LitMotionCpp
{
	MotionBuilder<float> LMotion::Create(float from, float to, float duration)
	{
		return MotionBuilder<float>(from, to, duration);
	}
}