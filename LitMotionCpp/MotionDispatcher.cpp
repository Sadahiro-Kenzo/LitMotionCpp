#include "pch.h"
#include <format>
#include <string>
#include "MotionDispatcher.h"
#include "MotionScheduler.h"

namespace LitMotionCpp
{
	std::map<int, FastListCore<std::weak_ptr<IUpdateRunner>>> MotionDispatcher::fastUpdateRunners;

	static double s_time = 0.0;
	double MotionDispatcher::getTime()
	{
		return s_time;
	}

	void MotionDispatcher::setTime(double value)
	{
		s_time = value;
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
			i.lock()->update(s_time, s_time, s_time);
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