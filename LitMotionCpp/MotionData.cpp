#include "pch.h"
#include "MotionData.h"
#include "IMotionScheduler.h"
#include "MotionStatus.h"

namespace LitMotionCpp
{
	MotionDataCore::MotionDataCore()
		:Status(MotionStatus::None)
		,Time(0.0)
		,PlaybackSpeed(1.0f)
		,Duration(0.0f)
		,Ease(Ease::Linear)
		,TimeKind(MotionTimeKind::Time)
		,Delay(0.0f)
		,Loops(1)
		,DelayType(DelayType::FirstLoop)
		,LoopType(LoopType::Restart)
	{

	}

}//namespace
