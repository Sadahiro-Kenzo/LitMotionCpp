#pragma once
namespace LitMotionCpp
{
	enum class MotionStatus;
	enum class Ease;
	enum class MotionTimeKind;
	enum class DelayType;
	enum class LoopType;
	class AnimationCurve;

	struct MotionDataCore
	{
		MotionStatus Status;
		double Time;
		float PlaybackSpeed;
		float Duration;
		Ease Ease;
		AnimationCurve* Curve;
		MotionTimeKind TimeKind;
		float Delay;
		int Loops;
		DelayType DelayType;
		LoopType LoopType;

		MotionDataCore(float duration);
		MotionDataCore();
	};

	template<typename TValue>
	struct MotionData
	{
		MotionDataCore Core;
		TValue StartValue;
		TValue EndValue;

		MotionData(const TValue& startValue, const TValue& endValue,const float duration)
			:Core{duration}
			, StartValue{ startValue }
			, EndValue{ endValue }
		{

		}

		MotionData()
			:Core{}
			,StartValue{}
			,EndValue{}
		{

		}
	};

}//namespace

