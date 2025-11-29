#include "pch.h"
#include <format>
#include <string>
#include "MotionDispatcher.h"
#include "MotionScheduler.h"

namespace LitMotionCpp
{
	std::map<int, FastListCore<std::weak_ptr<IUpdateRunner>>> MotionDispatcher::fastUpdateRunners;

	static double s_time = 0.0;
	static double s_unscaledTime = 0.0;
	static double s_prevRealtimeSinceStartup = 0.0;
	static double s_realtimeSinceStartup = 0.0;
	static double s_timeScale = 1.0;
	static double s_maxDeltaTime = 0.0333333; // 1/30 sec

	void MotionDispatcher::initializeTime()
	{
		s_time = 0.0;
		s_unscaledTime = 0.0;
		s_prevRealtimeSinceStartup = 0.0;
		s_realtimeSinceStartup = 0.0;
	}

	double MotionDispatcher::getTime()
	{
		return s_time;
	}

	double MotionDispatcher::getUnscaledTime()
	{
		return s_unscaledTime;
	}

	double MotionDispatcher::getRealtimeSinceStartup()
	{
		return s_realtimeSinceStartup;
	}

	void MotionDispatcher::setRealtimeSinceStartup(double value)
	{
		assert(value >= s_prevRealtimeSinceStartup);
		auto deltaTime = value - s_prevRealtimeSinceStartup;
		s_time += std::min(s_maxDeltaTime,deltaTime) * s_timeScale;
		s_unscaledTime += deltaTime;
		s_prevRealtimeSinceStartup = s_realtimeSinceStartup;
		s_realtimeSinceStartup = value;
	}

	double MotionDispatcher::getTimeScale()
	{
		return s_timeScale;
	}

	void MotionDispatcher::setTimeScale(double value)
	{
		assert(value >= 0.0);
		s_timeScale = value;
	}

	double MotionDispatcher::getMaxDeltaTime()
	{
		return s_maxDeltaTime;
	}

	void MotionDispatcher::setMaxDeltaTime(double value)
	{
		assert(value > 0.0);
		s_maxDeltaTime = value;
	}

	void MotionDispatcher::update(const MotionScheduler& scheduler)
	{
		if (fastUpdateRunners.empty())
		{
			return;
		}

		auto span = fastUpdateRunners[scheduler.getPlayerLoopTiming()].asSpan();
		for (auto& i : span)
		{
			i.lock()->update(s_time, s_unscaledTime, s_realtimeSinceStartup);
		}
	}

	void MotionDispatcher::clear()
	{
		for (auto& runners : fastUpdateRunners)
		{
			auto span = runners.second.asSpan();
			for (auto& i : span)
			{
				i.lock()->reset();
			}
		}
	}

	static std::string s_lastError;

	const char* MotionDispatcher::getLastError()
	{
		return s_lastError.c_str();
	}

	void defaultUnhandledExceptionHandler(std::exception& exception)
	{
		s_lastError = std::format("Exception: {}", exception.what());
	}

	static std::function<void(std::exception&)> s_unhandledExceptionHandler = defaultUnhandledExceptionHandler;

	void MotionDispatcher::registerUnhandledExceptionHandler(std::function<void(std::exception&)> unhandledExceptionHandler)
	{
		s_unhandledExceptionHandler = unhandledExceptionHandler;
	}

	std::function<void(std::exception&)> MotionDispatcher::getUnhandledExceptionHandler()
	{
		return s_unhandledExceptionHandler;
	}

	void invokeUnhandledExceptionHandler(std::exception& exception)
	{
		if (s_unhandledExceptionHandler)
		{
			s_unhandledExceptionHandler(exception);
		}
	}

}//namespace