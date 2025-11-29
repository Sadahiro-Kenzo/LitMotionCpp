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
	constexpr auto deltaTime = 1.0f / 60.0f;
	MotionDispatcher::initializeTime();

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

	auto old1 = target1.Value;
	auto old2 = target2.Value;

	MotionDispatcher::setRealtimeSinceStartup(MotionDispatcher::getRealtimeSinceStartup()+deltaTime);

	MotionDispatcher::update(scheduler1);
	EXPECT_NE(target1.Value, old1);			// updated
	EXPECT_FLOAT_EQ(target2.Value, old2);	// not updated
	old1 = target1.Value;

	MotionDispatcher::update(scheduler2);
	EXPECT_FLOAT_EQ(target1.Value, old1);	// not updated	
	EXPECT_NE(target2.Value, old2);			// updated
	old2 = target2.Value;

	MotionDispatcher::setRealtimeSinceStartup(MotionDispatcher::getRealtimeSinceStartup() + deltaTime);

	MotionDispatcher::update(scheduler1);
	EXPECT_NE(target1.Value, old1);			// updated
	EXPECT_FLOAT_EQ(target2.Value, old2);	// not updated
	old1 = target1.Value;

	MotionDispatcher::update(scheduler2);
	EXPECT_FLOAT_EQ(target1.Value, old1);	// not updated	
	EXPECT_NE(target2.Value, old2);			// updated
}