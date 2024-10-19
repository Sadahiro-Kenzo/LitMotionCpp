#include "pch.h"
#include <LMotion.h>

using namespace LitMotionCpp;

class CallbackTest :public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
		MotionScheduler::setDefault<float>(MotionScheduler::getManual<float>().lock());
	}
};

TEST_F(CallbackTest, Test_OnCancel)
{
	ManualMotionDispatcher::reset();

	auto canceled = false;
	auto handle = LMotion::create(0.0f, 10.0f, 2.0f)
		.withOnCancel([&canceled]() {canceled = true; })
		.runWithoutBinding();

	ManualMotionDispatcher::update(0.5f);

	handle.cancel();

	EXPECT_TRUE(canceled);
}

TEST_F(CallbackTest, Test_OnComplete)
{
	ManualMotionDispatcher::reset();
	
	auto completed = false;
	auto handle = LMotion::create(0.0f, 10.0f, 2.0f)
		.withOnComplete([&completed]() {completed = true; })
		.runWithoutBinding();

	ManualMotionDispatcher::update(2.1f);

	EXPECT_TRUE(completed);
}

TEST_F(CallbackTest, Test_CreateOnCallback)
{
	ManualMotionDispatcher::reset();

	auto created = false;
	auto completed = false;
	auto handle = LMotion::create(0.0f, 10.0f, 1.0f)
		.bind([&created, &completed](float x)
			{
				if (x > 5.0f && !created)
				{
					LMotion::create(0.0f, 10.0f, 1.0f)
						.withOnComplete([&completed]() {completed = true; })
						.runWithoutBinding();
					created = true;
				}
			});

	ManualMotionDispatcher::update(1.1f);
	ManualMotionDispatcher::update(1.1f);

	EXPECT_TRUE(created);
	EXPECT_TRUE(completed);
}

#ifdef LIT_MOTION_CPP_ENABLE_EXCEPTION
TEST_F(CallbackTest, Test_CompleteOnCallback_Self)
{
	try
	{
		MotionHandle handle;
		handle = LMotion::create(0.0f, 10.0f, 1.0f)
			.withOnComplete([&handle]() {handle.complete(); })
			.runWithoutBinding();
		handle.complete();
	}
	catch (std::runtime_error& exception)
	{
		EXPECT_STREQ("Recursion of Complete call was detected.", exception.what());
	}
}

TEST_F(CallbackTest, Test_CompleteOnCallback_CircularReference)
{
	try
	{
		MotionHandle handle1;
		MotionHandle handle2;
		handle1 = LMotion::create(0.0f, 10.0f, 1.0f)
			.withOnComplete([&handle2]() {handle2.complete(); })
			.runWithoutBinding();
		handle2 = LMotion::create(0.0f, 10.0f, 1.0f)
			.withOnComplete([&handle1]() {handle1.complete(); })
			.runWithoutBinding();

		handle1.complete();
	}
	catch (std::runtime_error& exception)
	{
		EXPECT_STREQ("Recursion of Complete call was detected.", exception.what());
	}
}
#else
TEST_F(CallbackTest, Test_CompleteOnCallback_Self)
{
	EXPECT_DEATH(
		{
			MotionHandle handle;
			handle = LMotion::create(0.0f, 10.0f, 1.0f)
				.withOnComplete([&handle]() {handle.complete(); })
				.runWithoutBinding();
			handle.complete();
		}, "");
}

TEST_F(CallbackTest, Test_CompleteOnCallback_CircularReference)
{
	EXPECT_DEATH(
		{
			MotionHandle handle1;
			MotionHandle handle2;
			handle1 = LMotion::create(0.0f, 10.0f, 1.0f)
				.withOnComplete([&handle2]() {handle2.complete(); })
				.runWithoutBinding();
			handle2 = LMotion::create(0.0f, 10.0f, 1.0f)
				.withOnComplete([&handle1]() {handle1.complete(); })
				.runWithoutBinding();

			handle1.complete();
		}, "");
}
#endif
TEST_F(CallbackTest, Test_CompleteOnCallback_Other)
{
	auto otherHandle = LMotion::create(0.0f, 10.0f, 5.0f).runWithoutBinding();
	LMotion::create(0.0f, 10.0f, 0.5f)
		.withOnComplete([&otherHandle]() {otherHandle.complete(); })
		.runWithoutBinding();

	ManualMotionDispatcher::update(0.3f);
	EXPECT_TRUE(otherHandle.isActive());
	ManualMotionDispatcher::update(0.7f);
	EXPECT_FALSE(otherHandle.isActive());
}

#ifdef LIT_MOTION_CPP_ENABLE_EXCEPTION
TEST_F(CallbackTest, Test_ThorwExceptionInsideCallbck)
{
	LMotion::create(0.0f, 10.0f, 0.5f)
		.withOnComplete([]() {throw std::exception{ "Test" }; })
		.runWithoutBinding();

	ManualMotionDispatcher::update(0.6f);
	EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
}

TEST_F(CallbackTest, Test_ThorwExceptionInsideCallbck_ThenCompleteManually)
{
	auto handle=LMotion::create(0.0f, 10.0f, 0.5f)
		.withOnComplete([]() {throw std::exception{ "Test" }; })
		.runWithoutBinding();

	handle.complete();
	EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
}

TEST_F(CallbackTest, Test_WithCancelOnError)
{
	bool completed = false;
	LMotion::create(0.0f, 10.0f, 0.5f)
		.withCancelOnError()
		.withOnComplete([&completed]() {completed = true; })
		.bind([](float x) {throw std::exception("Test"); });

	ManualMotionDispatcher::update(0.7f);
	EXPECT_FALSE(completed);
}

TEST_F(CallbackTest, Test_ThorwExceptionInsideBind_ThenCompleteManually)
{
	auto handle=LMotion::create(0.0f, 10.0f, 0.5f)
		.bind([](float x) {throw std::exception("Test"); });

	handle.complete();
	EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
}

TEST_F(CallbackTest, Test_RegisterUnhandledExceptionHandler)
{
	std::string lastError;

	auto defaultHandler = MotionDispatcher::getUnhandledExceptionHandler();
	MotionDispatcher::registerUnhandledExceptionHandler([&lastError](std::exception& exception) {lastError = exception.what(); });

	LMotion::create(0.0f, 10.0f, 0.5f)
		.withOnComplete([]() {throw std::exception{ "Test" }; })
		.runWithoutBinding();

	ManualMotionDispatcher::update(0.7f);
	EXPECT_STREQ("Test", lastError.c_str());

	MotionDispatcher::registerUnhandledExceptionHandler(defaultHandler);
}
#else
TEST_F(CallbackTest, Test_ThorwExceptionInsideCallbck)
{
	EXPECT_THROW(
		{
			LMotion::create(0.0f, 10.0f, 0.5f)
				.withOnComplete([]() {throw std::exception{ "Test" }; })
				.runWithoutBinding();

			ManualMotionDispatcher::update(0.6f);
			EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
		}, std::exception
	);
}

TEST_F(CallbackTest, Test_ThorwExceptionInsideCallbck_ThenCompleteManually)
{
	EXPECT_THROW(
		{
			auto handle = LMotion::create(0.0f, 10.0f, 0.5f)
				.withOnComplete([]() {throw std::exception{ "Test" }; })
				.runWithoutBinding();

			handle.complete();
			EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
		},std::exception
	);
}

TEST_F(CallbackTest, Test_WithCancelOnError)
{
	EXPECT_THROW(
		{
			bool completed = false;
			LMotion::create(0.0f, 10.0f, 0.5f)
				.withCancelOnError()
				.withOnComplete([&completed]() {completed = true; })
				.bind([](float x) {throw std::exception("Test"); });

			ManualMotionDispatcher::update(0.7f);
			EXPECT_FALSE(completed);
		}, std::exception
	);
}

TEST_F(CallbackTest, Test_ThorwExceptionInsideBind_ThenCompleteManually)
{
	EXPECT_THROW(
		{
			auto handle = LMotion::create(0.0f, 10.0f, 0.5f)
				.bind([](float x) {throw std::exception("Test"); });

			handle.complete();
			EXPECT_STREQ("Exception: Test", MotionDispatcher::getLastError());
		}, std::exception
	);
}

TEST_F(CallbackTest, Test_RegisterUnhandledExceptionHandler)
{
	EXPECT_THROW(
		{
			std::string lastError;

			auto defaultHandler = MotionDispatcher::getUnhandledExceptionHandler();
			MotionDispatcher::registerUnhandledExceptionHandler([&lastError](std::exception& exception) {lastError = exception.what(); });
			try
			{
				LMotion::create(0.0f, 10.0f, 0.5f)
					.withOnComplete([]() {throw std::exception{ "Test" }; })
					.runWithoutBinding();

				ManualMotionDispatcher::update(0.7f);
				EXPECT_STREQ("Test", lastError.c_str());

				MotionDispatcher::registerUnhandledExceptionHandler(defaultHandler);
			}
			catch (std::exception& exception)
			{
				MotionDispatcher::registerUnhandledExceptionHandler(defaultHandler);
				throw;
			}
		}, std::exception
	);
}
#endif