#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

class TestClass
{
public:
	float Value = 0.0f;
};

TEST(SchedulerTest, Test_PlayerLoopTiming)
{
	MotionScheduler scheduler1(MotionScheduler::createPlayerLoop(1));
	MotionScheduler scheduler2(MotionScheduler::createPlayerLoop(2));

	TestClass target1;
	TestClass target2;

	LMotion::create(0.0f, 10.0f, 2.0f)
		.withScheduler(scheduler1)
		.bind<TestClass>(&target1, [](float x, TestClass* pTarget) {pTarget->Value = x; });

	LMotion::create(0.0f, 10.0f, 2.0f)
		.withScheduler(scheduler2)
		.bind<TestClass>(&target2, [](float x, TestClass* pTarget) {pTarget->Value = x; });

	MotionDispatcher::setTime(1.0);

	MotionDispatcher::update(scheduler1);
	EXPECT_FLOAT_EQ(target1.Value, 5.0f);
	EXPECT_FLOAT_EQ(target2.Value, 0.0f);

	MotionDispatcher::update(scheduler2);
	EXPECT_FLOAT_EQ(target1.Value, 5.0f);
	EXPECT_FLOAT_EQ(target2.Value, 5.0f);

	MotionDispatcher::setTime(2.1);

	MotionDispatcher::update(scheduler1);
	EXPECT_FLOAT_EQ(target1.Value, 10.0f);
	EXPECT_FLOAT_EQ(target2.Value, 5.0f);

	MotionDispatcher::update(scheduler2);
	EXPECT_FLOAT_EQ(target1.Value, 10.0f);
	EXPECT_FLOAT_EQ(target2.Value, 10.0f);
}