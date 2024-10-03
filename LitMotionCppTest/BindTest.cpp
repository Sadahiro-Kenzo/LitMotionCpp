#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

class TestClass
{
public:
	float Value = 0.0f;
};

TEST(BindTest, Test_Bind_LocalVariable)
{
	ManualMotionDispatcher::reset();

	auto value = 0.0f;
	auto endValue = 10.0f;

	auto handle = LMotion::Create(0.0f, endValue, 1.0f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bind([&value](float x) {value = x; });

	ManualMotionDispatcher::update(1.1f);

	EXPECT_FLOAT_EQ(value, endValue);
}

TEST(BindTest, Test_BindWithState)
{
	ManualMotionDispatcher::reset();

	TestClass target;
	auto endValue = 10.0f;

	auto handle = LMotion::Create(0.0f, endValue, 0.5f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bindWithState<TestClass>(&target,[](float x,TestClass* pTarget) {pTarget->Value = x; });

	ManualMotionDispatcher::update(0.6f);

	EXPECT_FLOAT_EQ(target.Value, endValue);
}

TEST(BindTest, Test_BindWithState_2)
{
	ManualMotionDispatcher::reset();

	TestClass target1;
	TestClass target2;
	auto endValue = 10.0f;

	auto handle = LMotion::Create(0.0f, endValue, 0.5f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bindWithState<TestClass,TestClass>(&target1,&target2, [](float x, TestClass* pTarget1,TestClass* pTarget2)
			{
				pTarget1->Value = x;
				pTarget2->Value = x;
			});

	ManualMotionDispatcher::update(0.6f);

	EXPECT_FLOAT_EQ(target1.Value, endValue);
	EXPECT_FLOAT_EQ(target2.Value, endValue);
}

TEST(BindTest, Test_BindWithState_3)
{
	ManualMotionDispatcher::reset();

	TestClass target1;
	TestClass target2;
	TestClass target3;
	auto endValue = 10.0f;

	auto handle = LMotion::Create(0.0f, endValue, 0.5f)
		.withScheduler(MotionScheduler::getManual<float>())
		.bindWithState<TestClass, TestClass, TestClass>(&target1,&target2,&target3, [](float x, TestClass* pTarget1, TestClass* pTarget2, TestClass* pTarget3)
			{
				pTarget1->Value = x;
				pTarget2->Value = x;
				pTarget3->Value = x;
			});

	ManualMotionDispatcher::update(0.6f);

	EXPECT_FLOAT_EQ(target1.Value, endValue);
	EXPECT_FLOAT_EQ(target2.Value, endValue);
	EXPECT_FLOAT_EQ(target3.Value, endValue);
}