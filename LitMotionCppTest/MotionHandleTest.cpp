#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

TEST(MotionHandleTest, Test_Cancel)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto endValue = 10.0f;

	auto handle = LMotion::create(value, endValue, 2.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bind([&value](float x) {value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.cancel();
	ManualMotionDispatcher::update(1.0f);

	EXPECT_LT(value, endValue);
	EXPECT_FALSE(handle.isActive());
}

TEST(MotionHandleTest, Test_Completel)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto endValue = 10.0f;

	auto handle = LMotion::create(value, endValue, 2.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bind([&value](float x) {value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.complete();

	EXPECT_FLOAT_EQ(value, endValue);
	EXPECT_FALSE(handle.isActive());
}
