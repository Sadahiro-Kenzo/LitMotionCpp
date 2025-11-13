#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

struct State
{
	float value = 0.0f;
};

TEST(MotionHandleTest, Test_Cancel)
{
	ManualMotionDispatcher::reset();

	State state;
	auto endValue = 10.0f;

	auto handle = LMotion::create(state.value, endValue, 2.0f)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.cancel();
	ManualMotionDispatcher::update(1.0f);

	EXPECT_LT(state.value, endValue);
	EXPECT_FALSE(handle.isActive());
}

TEST(MotionHandleTest, Test_Complete)
{
	ManualMotionDispatcher::reset();

	State state;
	auto endValue = 10.0f;

	auto handle = LMotion::create(state.value, endValue, 2.0f)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.complete();

	EXPECT_FLOAT_EQ(state.value, endValue);
	EXPECT_FALSE(handle.isActive());
}

TEST(MotionHandleTest, Test_Complete_WithYoyoLoop)
{
	ManualMotionDispatcher::reset();

	State state;
	auto startValue = 0.0f;
	auto handle = LMotion::create(startValue, 10.0f, 2.0f)
		.withLoops(2, LoopType::Yoyo)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	ManualMotionDispatcher::update(1.0f);
	handle.complete();

	EXPECT_FLOAT_EQ(state.value, startValue);
	EXPECT_TRUE(!handle.isActive());
}

TEST(MotionHandleTest, Test_CompleteAndCancel_WithInfiniteLoop)
{
	ManualMotionDispatcher::reset();

	auto startValue = 0.0f;
	auto handle = LMotion::create(startValue, 10.0f, 2.0f)
		.withLoops(-1)
		.runWithoutBinding();

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
		.runWithoutBinding();

	EXPECT_TRUE(handle.isActive());
	ManualMotionDispatcher::update(2.5f);
	EXPECT_FALSE(handle.isActive());
}
