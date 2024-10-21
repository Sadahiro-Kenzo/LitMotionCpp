#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

TEST(BindOnScheduleTest, Test_BindOnSchedule)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;

	auto handle = LMotion::create(1.0f, 0.0f, 1.0f)
		.bind([&value](float x) {value = x; });

	handle.cancel();

	EXPECT_EQ(value, 0.0f);

	value = 0.0f;

	handle = LMotion::create(1.0f, 0.0f, 1.0f)
		.withBindOnSchedule()
		.bind([&value](float x) {value = x; });

	handle.cancel();

	EXPECT_FLOAT_EQ(value, 1.0f);
}
