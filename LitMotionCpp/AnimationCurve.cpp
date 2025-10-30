#include "pch.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include "AnimationCurve.h"

namespace LitMotionCpp
{
	/*
	* @brief ping pong function
	*/
	float IAnimationCurve::pingPong(const float t, const float length)
	{
		const int n = static_cast<int>(std::floorf(t / length));
		if (n % 2 == 1)
		{
			return length - (t - n * length);
		}
		else
		{
			return t - n * length;
		}
	}

	/*
	* @brief wrap time according to wrap modes
	*/
	float IAnimationCurve::wrapTime(const float startTime, const float endTime, WrapMode preWrapMode, WrapMode postWrapMode, const float time)
	{
		if (time < startTime)
		{
			// pre wrap
			switch (preWrapMode)
			{
			case WrapMode::Loop:	return endTime - std::fmod(startTime - time, endTime - startTime);
			case WrapMode::PingPong:return startTime + pingPong(startTime - time, endTime - startTime);
			case WrapMode::Once:
			case WrapMode::Default:
			case WrapMode::ClampForever:
			default:
				return startTime;
			}
		}
		else if (time > endTime)
		{
			// post wrap
			switch (postWrapMode)
			{
			case WrapMode::Loop:	return std::fmod(time - startTime, endTime - startTime);
			case WrapMode::PingPong:return startTime + pingPong(time - startTime, endTime - startTime);
			case WrapMode::Once:
			case WrapMode::Default:
			case WrapMode::ClampForever:
			default:
				return endTime;
			}
		}

		return time;
	}

	/*
	* @brief default factory function for creating AnimationCurve instances
	*/
	AnimationCurve* defaultFactory(const Keyframe* begin, const Keyframe* end,WrapMode preWrapMode, WrapMode postWrapMode)
	{
		return new AnimationCurve(begin, end,preWrapMode,postWrapMode);
	}

	/*
	* @brief current factory function for creating AnimationCurve instances
	*/
	std::function<AnimationCurve* (const Keyframe*, const Keyframe*,WrapMode,WrapMode)> g_factory = defaultFactory;

	/*
	* @brief create AnimationCurve instance using the current factory function
	*/
	AnimationCurve* AnimationCurve::create(const Keyframe* begin, const Keyframe* end,WrapMode preWrapMode,WrapMode postWrapMode)
	{
		assert(std::is_sorted(begin, end, [](const auto& left, const auto& right) { return left.time < right.time; }) && "keyframes must be sorted by time");
		return g_factory(begin, end,preWrapMode,postWrapMode);
	}

	/*
	* @brief set factory function for creating AnimationCurve instances
	*/
	void AnimationCurve::setFactory(std::function<AnimationCurve* (const Keyframe*, const Keyframe*,WrapMode,WrapMode)> factory)
	{
		if(factory)
		{
			g_factory = factory;
		}
		else
		{
			g_factory = defaultFactory;
		}
	}

	/*
	* @brief interpolation between two keyframes
	*/
	float AnimationCurve::interpolate(float time, const Keyframe& keyframe, const Keyframe& nextKeyframe) const
	{
		if ((keyframe.weightedMode == WeightedMode::None || keyframe.weightedMode == WeightedMode::In) &&
			(nextKeyframe.weightedMode==WeightedMode::None||nextKeyframe.weightedMode==WeightedMode::Out))
		{
			// Hermite
			// https://en.wikipedia.org/wiki/Cubic_Hermite_spline
			const float s = nextKeyframe.time - keyframe.time;
			const float t = (time - keyframe.time) / s;
			const float t2 = t * t;
			const float t3 = t2 * t;
			const float h00 = 2 * t3 - 3 * t2 + 1;
			const float h10 = t3 - 2 * t2 + t;
			const float h01 = -2 * t3 + 3 * t2;
			const float h11 = t3 - t2;
			const float value = h00 * keyframe.value + h10 * s * keyframe.outTangent + h01 * nextKeyframe.value + h11 * s * nextKeyframe.inTangent;
			return value;
		}

		// Bezier
		// https://en.wikipedia.org/wiki/B%C3%A9zier_curve
		const float s = nextKeyframe.time - keyframe.time;
		const float t = (time - keyframe.time) / s;
		const float t2 = t * t;
		const float t3 = t2 * t;
		const float f0 = -t3 + 3 * t2 - 3 * t + 1;
		const float f1 = 3 * t3 - 6 * t2 + 3 * t;
		const float f2 = -3 * t3 + 3 * t2;
		const float f3 = t3;
		const float value= f0 * keyframe.value
			+ f1 * (keyframe.value + keyframe.outWeight * keyframe.outTangent * s)
			+ f2 * (nextKeyframe.value - nextKeyframe.inWeight * nextKeyframe.inTangent * s)
			+ f3 * nextKeyframe.value;
		return value;
	}

	/*
	* @brief evaluate the curve at the given time
	*/
	float AnimationCurve::evaluate(float time) const
	{
		time = wrapTime(m_begin->time,(m_end-1)->time,m_preWrapMode,m_postWrapMode, time);

		auto i=std::lower_bound(
			m_begin,
			m_end,
			Keyframe{ time,-1 },
			[](const auto& left,const auto& right)
			{
				return left.time < right.time;
			});
		if(i==m_begin)
		{
			i++;
		}
		else if(i==m_end)
		{
			i--;
		}

		const Keyframe& keyframe = *(i-1);
		const Keyframe& nextKeyframe = *i;

		if (!std::isfinite(keyframe.outTangent) || !std::isfinite(nextKeyframe.inTangent))
		{
			return keyframe.value;
		}

		// interpolation
		return interpolate(time, keyframe,nextKeyframe);
	}
}// namespace


