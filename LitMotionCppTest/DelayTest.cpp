#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

class DelayTest :public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
		MotionScheduler::setDefault<float>(MotionScheduler::getManual<float>().lock());
	}

	void play(MotionHandle handle)
	{
		constexpr float delta = 1.0f / 60.0f;
		ManualMotionDispatcher::update(delta);
		while (handle.isActive())
		{
			ManualMotionDispatcher::update(delta);
		}
	}
};

TEST_F(DelayTest, Test_Delay)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	play(LMotion::create(0.0f, 1.0f, 0.5f)
		.withDelay(0.5f)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(0.95<duration && duration<1.1);
}

TEST_F(DelayTest, Test_Delay_WithZeroDuration)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	play(LMotion::create(0.0f, 1.0f, 0.0f)
		.withDelay(1.0f)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(0.95 < duration && duration < 1.1);
}

TEST_F(DelayTest, Test_Delay_EveryLoop)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	play(LMotion::create(0.0f, 1.0f, 0.5f)
		.withLoops(2)
		.withDelay(0.5f,DelayType::EveryLoop)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(1.95 < duration && duration < 2.1);
}

TEST_F(DelayTest, Test_Delay_EveryLoop_WithZeroDuration)
{
	ManualMotionDispatcher::reset();

	auto t = ManualMotionDispatcher::getTime();
	play(LMotion::create(0.0f, 1.0f, 0.0f)
		.withLoops(3)
		.withDelay(0.5f, DelayType::EveryLoop)
		.runWithoutBinding()
	);

	const auto duration = ManualMotionDispatcher::getTime() - t;
	EXPECT_TRUE(1.45 < duration && duration < 1.6);
}

TEST_F(DelayTest, Test_Delay_SkipValuesDuringDelay)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto handle = LMotion::create(1.0f, 2.0f, 0.5f)
		.withDelay(0.5f, DelayType::FirstLoop, false)
		.bind([&value](float x) {value = x; });
	ManualMotionDispatcher::update(0.1f);
	EXPECT_TRUE(0.9f < value);

	handle.cancel();
	value = 0.0f;
	handle = LMotion::create(1.0f, 2.0f, 0.5f)
		.withDelay(0.5f, DelayType::FirstLoop, true)
		.bind([&value](float x) {value = x; });
	ManualMotionDispatcher::update(0.1f);
	EXPECT_TRUE(0.9f > value);
	handle.cancel();
}
