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

TEST(MotionHandleTest, Test_Complete)
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

TEST(MotionHandleTest, Test_Complete_WithYoyoLoop)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto startValue = 0.0f;
	auto handle = LMotion::create(startValue, 10.0f, 2.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.withLoops(2, LoopType::Yoyo)
		.bind([&value](float x) {value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.complete();

	EXPECT_FLOAT_EQ(value, startValue);
	EXPECT_TRUE(!handle.isActive());
}

TEST(MotionHandleTest, Test_CompleteAndCancel_WithInfiniteLoop)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto startValue = 0.0f;
	auto handle = LMotion::create(startValue, 10.0f, 2.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.withLoops(-1)
		.bind([&value](float x) {value = x; });

	ManualMotionDispatcher::update(1.0f);

	handle.complete();
	EXPECT_TRUE(handle.isActive());
	handle.cancel();
	EXPECT_TRUE(!handle.isActive());
}

TEST(MotionHandleTest, Test_IsActive)
{
	ManualMotionDispatcher::reset();

	auto handle = LMotion::create(0.0f, 10.0f, 2.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.runWithoutBinding();

	EXPECT_TRUE(handle.isActive());
	ManualMotionDispatcher::update(2.5f);
	EXPECT_FALSE(handle.isActive());
}
