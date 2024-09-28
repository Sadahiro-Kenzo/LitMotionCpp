#pragma once
namespace LitMotionCpp
{
	enum class Ease;

	/**
	* @brief Utility class that provides calculation of easing functions.
	*/
	class EaseUtility
	{
	public:
		static float evaluate(float, Ease);
		static float linear(float);
		static float inSine(float);
		static float outSine(float);
		static float inOutSine(float);
		static float inQuad(float);
		static float outQuad(float);
		static float inOutQuad(float);
		static float inCublic(float);
		static float outCublic(float);
		static float inOutCublic(float);
		static float inQuart(float);
		static float outQuart(float);
		static float inOutQuart(float);
		static float inQuint(float);
		static float outQuint(float);
		static float inOutQuint(float);
		static float inExpo(float);
		static float outExpo(float);
		static float inOutExpo(float);
		static float inCirc(float);
		static float outCirc(float);
		static float inOutCirc(float);
		static float inBack(float);
		static float outBack(float);
		static float inOutBack(float);
		static float inElastic(float);
		static float outElastic(float);
		static float inOutElastic(float);
		static float inBounce(float);
		static float outBounce(float);
		static float inOutBounce(float);
	};

}//namespace

