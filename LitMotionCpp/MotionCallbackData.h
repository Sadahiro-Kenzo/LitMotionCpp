#pragma once
#include <functional>

namespace LitMotionCpp
{
	struct MotionCallbackDataCore
	{
		uint8_t StateCount = 0;
		bool IsCallbackRunning = false;
		bool CancelOnError = false;
		bool SkipValuesDuringDelay = true;
		std::function<void()> OnCompleteAction;
		std::function<void()> OnCancelAction;
	};

	template<typename TValue>
	struct MotionCallbackData:public MotionCallbackDataCore
	{
		std::function<void(TValue&)> UpdateAction;
	};
}//namespace