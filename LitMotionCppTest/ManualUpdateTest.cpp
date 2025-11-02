#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

TEST(ManualUpdateTest, Test_FloatMotion)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto endValue = 10.0f;

	auto handle=LMotion::create(value, endValue, 2.0f)
		.withScheduler(MotionScheduler::getManual<float,NoOptions,FloatMotionAdapter<float>>())
		.bind([&value](float x) {value = x; });

	while (handle.isActive())
	{
		ManualMotionDispatcher::update(0.1f);
	}

	EXPECT_FLOAT_EQ(value, endValue);
}