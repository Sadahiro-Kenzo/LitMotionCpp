#pragma once
#include <memory>

namespace LitMotionCpp
{
	class IMotionStorage;
	struct MotionDataCore;
	struct MotionCallbackDataCore;
	struct MotionHandle;

	class MotionStorageManager
	{
	public:
		static int getCurrentStorageId();
		static void addStorage(std::weak_ptr<IMotionStorage> storage);
		static void completeMotion(const MotionHandle&);
		static void cancelMotion(const MotionHandle&);
		static bool isActive(const MotionHandle&);
		static MotionDataCore& getMotionDataRef(const MotionHandle&);
		static MotionCallbackDataCore& getMotionCallbackDataRef(const MotionHandle&);
	};

}//namespace
