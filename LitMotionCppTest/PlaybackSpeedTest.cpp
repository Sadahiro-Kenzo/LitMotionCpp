#include "pch.h"
#include <LMotion.h>
#include "Utility.h"

using namespace LitMotionCpp;

struct State
{
	float value = 0.0f;
};

TEST(PlaybackSpeedTest, Test_PlaybackSpeed)
{
	ManualMotionDispatcher::reset();

	auto endValue = 10.0f;
	auto handle = LMotion::create(0.0f, endValue, 1.0f)
		.runWithoutBinding();
	handle.setPlaybackSpeed(0.5f);
	
	auto time = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(handle);

	EXPECT_TRUE(2.0f<ManualMotionDispatcher::getTime()-time);
}

TEST(PlaybackSpeedTest, Test_PlaybackSpeed_Pause)
{
	ManualMotionDispatcher::reset();

	auto endValue = 10.0f;
	State state;
	auto handle = LMotion::create(0.0f, endValue, 1.0f)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	handle.setPlaybackSpeed(0.0f);
	Utility::waitForSeconds(0.5f);
	EXPECT_FLOAT_EQ(0.0f, state.value);

	handle.cancel();
}

TEST(PlaybackSpeedTest, Test_PlaybackSpeed_2x_Speed)
{
	ManualMotionDispatcher::reset();

	auto endValue = 10.0f;
	State state;
	auto handle = LMotion::create(0.0f, endValue, 1.0f)
		.bind<State>(&state,[](float x,State* pState) {pState->value = x; });

	handle.setPlaybackSpeed(2.0f);
	auto time = ManualMotionDispatcher::getTime();
	Utility::toYieldInteraction(handle);
	EXPECT_NEAR(0.5f, ManualMotionDispatcher::getTime() - time,0.05f);
	EXPECT_FLOAT_EQ(endValue, state.value);
}

#ifdef _DEBUG
TEST(PlaybackSpeedTest, Test_PlaybackSpeed_Minus)
{
	auto handle = LMotion::create(0.0f, 10.0f, 1.0f).runWithoutBinding();
	EXPECT_THROW(
		{
			handle.setPlaybackSpeed(-1.0f);
		}, std::out_of_range
	);
}
#endif