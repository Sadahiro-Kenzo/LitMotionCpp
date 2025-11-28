#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

struct State
{
	float value = 0.0f;
};

TEST(ManualUpdateTest, Test_FloatMotion)
{
	ManualMotionDispatcher::reset();

	State state;
	auto endValue = 10.0f;

	auto handle=LMotion::create(state.value, endValue, 2.0f)
		.withScheduler(MotionScheduler::getManual())
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	while (handle.isActive())
	{
		ManualMotionDispatcher::update(0.1f);
	}

	EXPECT_FLOAT_EQ(state.value, endValue);
}