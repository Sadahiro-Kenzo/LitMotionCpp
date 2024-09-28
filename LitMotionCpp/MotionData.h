#pragma once
namespace LitMotionCpp
{
	enum class MotionStatus;
	enum class Ease;
	enum class MotionTimeKind;
	enum class DelayType;
	enum class LoopType;

	struct MotionDataCore
	{
		MotionStatus Status;
		double Time;
		float PlaybackSpeed;
		float Duration;
		Ease Ease;
		MotionTimeKind TimeKind;
		float Delay;
		int Loops;
		DelayType DelayType;
		LoopType LoopType;

		MotionDataCore();
	};

	template<typename TValue>
	struct MotionData
	{
		MotionDataCore Core;
		TValue StartValue;
		TValue EndValue;
	};

}//namespace

