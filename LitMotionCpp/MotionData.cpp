#include "pch.h"
#include "MotionData.h"
#include "IMotionScheduler.h"
#include "MotionStatus.h"

namespace LitMotionCpp
{
	MotionDataCore::MotionDataCore(float duration)
		:Status(MotionStatus::None)
		,Time(0.0)
		,PlaybackSpeed(1.0f)
		,Duration(duration)
		,Ease(Ease::Linear)
		,Curve(nullptr)
		,TimeKind(MotionTimeKind::Time)
		,Delay(0.0f)
		,Loops(1)
		,DelayType(DelayType::FirstLoop)
		,LoopType(LoopType::Restart)
	{

	}

	MotionDataCore::MotionDataCore()
		:MotionDataCore(0.0f)
	{

	}

}//namespace
