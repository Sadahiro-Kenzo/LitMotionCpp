#include "pch.h"
#include <LMotion.h>
#include "Utility.h"

using namespace LitMotionCpp;

struct State
{
	float value = 0.0f;
};

TEST(DelayTest, Test_Delay)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(LMotion::create(0.0f, 1.0f, 0.5f)
		.withDelay(0.5f)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(0.95<duration && duration<1.1);
}

TEST(DelayTest, Test_Delay_WithZeroDuration)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(LMotion::create(0.0f, 1.0f, 0.0f)
		.withDelay(1.0f)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(0.95 < duration && duration < 1.1);
}

TEST(DelayTest, Test_Delay_EveryLoop)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(LMotion::create(0.0f, 1.0f, 0.5f)
		.withLoops(2)
		.withDelay(0.5f,DelayType::EveryLoop)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(1.95 < duration && duration < 2.1);
}

TEST(DelayTest, Test_Delay_EveryLoop_WithZeroDuration)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(LMotion::create(0.0f, 1.0f, 0.0f)
		.withLoops(3)
		.withDelay(0.5f, DelayType::EveryLoop)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(1.45 < duration && duration < 1.6);
}

TEST(DelayTest, Test_Delay_SkipValuesDuringDelay)
{
	ManualMotionDispatcher::reset();

	State state;
	auto handle = LMotion::create(1.0f, 2.0f, 0.5f)
		.withDelay(0.5f, DelayType::FirstLoop, false)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });
	ManualMotionDispatcher::update(0.1f);
	EXPECT_TRUE(0.9f < state.value);

	handle.cancel();
	state.value = 0.0f;
	handle = LMotion::create(1.0f, 2.0f, 0.5f)
		.withDelay(0.5f, DelayType::FirstLoop, true)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });
	ManualMotionDispatcher::update(0.1f);
	EXPECT_TRUE(0.9f > state.value);
	handle.cancel();
}
