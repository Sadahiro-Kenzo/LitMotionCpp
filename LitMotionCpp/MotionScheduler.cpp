#include "pch.h"
#include "MotionScheduler.h"

namespace LitMotionCpp
{
	MotionScheduler s_defaultScheduler = MotionScheduler::getUpdate();
	MotionScheduler& MotionScheduler::getDefaultScheduler()
	{
		return s_defaultScheduler;
	}

	void MotionScheduler::setDefaultScheduler(const MotionScheduler& scheduler)
	{
		assert(scheduler.getType() != Type::Invalid && "Cannot set Invalid as default scheduler");
		s_defaultScheduler = scheduler;
	}

	MotionScheduler& MotionScheduler::getUpdate()
	{
		static MotionScheduler scheduler(0, MotionTimeKind::Time, Type::PlayerLoop);
		return scheduler;
	}

	MotionScheduler& MotionScheduler::getManual()
	{
		static MotionScheduler scheduler(0, MotionTimeKind::Time, Type::Manual);
		return scheduler;
	}

}//namespace
