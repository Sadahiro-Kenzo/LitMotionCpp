#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

TEST(BindOnScheduleTest, Test_BindOnSchedule)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;

	auto handle = LMotion::Create(1.0f, 0.0f, 1.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bind([&value](float x) {value = x; });

	handle.cancel();

	EXPECT_EQ(value, 0.0f);

	value = 0.0f;

	handle = LMotion::Create(1.0f, 0.0f, 1.0f)
		.withBindOnSchedule()
		.withScheduler(MotionScheduler::getManual<float>())
		.bind([&value](float x) {value = x; });

	handle.cancel();

	EXPECT_FLOAT_EQ(value, 1.0f);
}
