#include "pch.h"
#include <cmath>
#include <numbers>
#include "EaseUtility.h"
#include "MotionStatus.h"

namespace LitMotionCpp
{
	float EaseUtility::evaluate(float t, Ease ease)
	{
		switch (ease)
		{
		case LitMotionCpp::Ease::InSine:	return inSine(t);
		case LitMotionCpp::Ease::OutSine:	return outSine(t);
		case LitMotionCpp::Ease::InOutSine:	return inOutSine(t);
		case LitMotionCpp::Ease::InQuad:	return inQuad(t);
		case LitMotionCpp::Ease::OutQuad:	return outQuad(t);
		case LitMotionCpp::Ease::InOutQuad:	return inOutQuad(t);
		case LitMotionCpp::Ease::InCublic:	return inCublic(t);
		case LitMotionCpp::Ease::OutCublic:	return outCublic(t);
		case LitMotionCpp::Ease::InOUtCublic:	return inOutCublic(t);
		case LitMotionCpp::Ease::InQuart:	return inQuart(t);
		case LitMotionCpp::Ease::OutQuart:	return outQuart(t);
		case LitMotionCpp::Ease::InOutQuart:	return inOutQuart(t);
		case LitMotionCpp::Ease::InQuint:	return inQuint(t);
		case LitMotionCpp::Ease::OutQuint:	return outQuint(t);
		case LitMotionCpp::Ease::InOutQuint:	return inOutQuint(t);
		case LitMotionCpp::Ease::InExpo:	return inExpo(t);
		case LitMotionCpp::Ease::OutExpo:		return outExpo(t);
		case LitMotionCpp::Ease::InOutExpo:		return inOutExpo(t);
		case LitMotionCpp::Ease::InCirc:	return inCirc(t);
		case LitMotionCpp::Ease::OutCirc:	return outCirc(t);
		case LitMotionCpp::Ease::InOutCirc:	return inOutCirc(t);
		case LitMotionCpp::Ease::InElastic:	return inElastic(t);
		case LitMotionCpp::Ease::OutElastic:	return outElastic(t);
		case LitMotionCpp::Ease::InOutElastic:	return inOutElastic(t);
		case LitMotionCpp::Ease::InBack:	return inBack(t);
		case LitMotionCpp::Ease::OutBack:	return outBack(t);
		case LitMotionCpp::Ease::InOutBack:	return inOutBack(t);
		case LitMotionCpp::Ease::InBounce:	return inBounce(t);
		case LitMotionCpp::Ease::OutBounce:	return outBounce(t);
		case LitMotionCpp::Ease::InOutBounce:	return inOutBounce(t);
		default:	return t;
		}

	}

	float EaseUtility::linear(float t)
	{
		return t;
	}

	float EaseUtility::inSine(float t)
	{
		return 1.0f - std::cosf(t * std::numbers::pi_v<float> / 2.0f);
	}

	float EaseUtility::outSine(float t)
	{
		return std::sinf(t * std::numbers::pi_v<float> / 2.0f);
	}

	float EaseUtility::inOutSine(float t)
	{
		return - (std::cosf(t * std::numbers::pi_v<float>)-1.0f) / 2.0f;
	}

	float EaseUtility::inQuad(float t)
	{
		return t * t;
	}

	float EaseUtility::outQuad(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	float EaseUtility::inOutQuad(float t)
	{
		return t < 0.5f ? 2.0f * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 2) / 2.0f;
	}

	float EaseUtility::inCublic(float t)
	{
		return t * t * t;
	}

	float EaseUtility::outCublic(float t)
	{
		return 1.0f - std::powf(1.0f - t, 3);
	}

	float EaseUtility::inOutCublic(float t)
	{
		return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
	}

	float EaseUtility::inQuart(float t)
	{
		return t * t * t * t;
	}

	float EaseUtility::outQuart(float t)
	{
		return 1.0f - std::powf(1.0f - t, 4);
	}

	float EaseUtility::inOutQuart(float t)
	{
		return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;
	}

	float EaseUtility::inQuint(float t)
	{
		return t * t * t * t * t;
	}

	float EaseUtility::outQuint(float t)
	{
		return 1.0f - std::powf(1.0f - t, 5);
	}

	float EaseUtility::inOutQuint(float t)
	{
		return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;
	}

	float EaseUtility::inExpo(float t)
	{
		return t == 0.0f ? 0.0f : std::powf(2.0f, 10.0f * t - 10.0f);
	}

	float EaseUtility::outExpo(float t)
	{
		return t == 1.0f ? 1.0f : 1.0f - std::powf(2.0f, -10.0f * t);
	}

	float EaseUtility::inOutExpo(float t)
	{
		return t == 0.0f ? 0 :
			t == 1.0f ? 1 :
			t < 0.5f ? std::powf(2.0f, 20.0f * t - 10.0f) / 2.0f :
			(2.0f - std::powf(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	float EaseUtility::inCirc(float t)
	{
		return 1.0f - std::sqrtf(1.0f - std::powf(t, 2.0f));
	}

	float EaseUtility::outCirc(float t)
	{
		return std::sqrtf(1.0f - std::powf(t-1.0f, 2.0f));
	}

	float EaseUtility::inOutCirc(float t)
	{
		return t < 0.0f ?
			(1.0f - std::sqrtf(1.0f - std::powf(2.0f * t, 2.0f))) / 2.0f :
			(std::sqrtf(1.0f - std::powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}

	float EaseUtility::inBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;
		return c3 * t * t * t - c1 * t * t;
	}

	float EaseUtility::outBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;
		return 1.0f + c3 * std::powf(t - 1.0f, 3.0f) + c1 * std::powf(t - 1.0f, 2.0f);
	}

	float EaseUtility::inOutBack(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;

		return t < 0.5f
			? std::powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2) / 2.0f
			: (std::powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}

	float EaseUtility::inElastic(float t)
	{
		constexpr float c4 = 2.0f * std::numbers::pi_v<float> / 3.0f;

		return t == 0.0f ? 0.0f :
			t == 1.0f ? 1.0f :
			-powf(2.0f, 10.0f * t - 10.0f) * std::sinf((t * 10.0f - 10.75f) * c4);
	}

	float EaseUtility::outElastic(float t)
	{
		constexpr float c4 = 2.0f * std::numbers::pi_v<float> / 3.0f;

		return t == 0.0f ? 0.0f :
			t == 1.0f ? 1.0f :
			powf(2.0f, -10.0f * t) * std::sinf((t * 10.0f - 0.75f) * c4)+1.0f;
	}

	float EaseUtility::inOutElastic(float t)
	{
		constexpr float c5 = 2.0f * std::numbers::pi_v<float> / 4.5f;

		return t == 0.0f ? 0.0f :
			t == 1.0f ? 1.0f :
			t < 0.0f ?
			-(powf(2.0f, 20.0f * t - 10.0f) * std::sinf((t * 20.0f - 11.125f) * c5)) / 2.0f :
			powf(2.0f, -20.0f * t + 10.0f) * std::sinf((t * 20.0f - 11.125f) * c5) / 2.0f + 1.0f;
	}

	float EaseUtility::inBounce(float t)
	{
		return 1.0f - outBounce(1.0f - t);
	}

	float EaseUtility::outBounce(float t)
	{
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;

		if (t < 1.0f / d1)
		{
			return n1 * t * t;
		}
		else if (t < 2.0f / d1)
		{
			return n1 * (t -= 1.5f / d1) * t + 0.75f;
		}
		else if (t < 2.5f / d1)
		{
			return n1 * (t -= 2.25f / d1) * t + 0.9375f;
		}
		else
		{
			return n1 * (t -= 2.625f / d1) * t + 0.984375f;
		}
	}

	float EaseUtility::inOutBounce(float t)
	{
		return t < 0.0f ?
			(1.0f - outBounce(1.0f - 2.0f * t)) / 2.0f :
			(1.0f + outBounce(2.0f * t - 1.0f)) / 2.0f;
	}
}