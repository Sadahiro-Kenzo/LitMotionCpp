#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

struct State
{
	float value = 0.0f;
};

TEST(BindOnScheduleTest, Test_BindOnSchedule)
{
	ManualMotionDispatcher::reset();

	State state;

	auto handle = LMotion::create(1.0f, 0.0f, 1.0f)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	handle.cancel();

	EXPECT_EQ(state.value, 0.0f);

	state.value = 0.0f;

	handle = LMotion::create(1.0f, 0.0f, 1.0f)
		.withBindOnSchedule()
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	handle.cancel();

	EXPECT_FLOAT_EQ(state.value, 1.0f);
}
