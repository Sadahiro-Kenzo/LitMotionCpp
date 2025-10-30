#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

class Environment :public ::testing::Environment
{
public:
	virtual ~Environment() = default;
	virtual void SetUp() override
	{
		MotionScheduler::setDefault<float>(MotionScheduler::getManual<float,NoOptions>().lock());
	}
};

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new Environment());
	return RUN_ALL_TESTS();
}