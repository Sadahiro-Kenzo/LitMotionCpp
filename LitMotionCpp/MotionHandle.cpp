#include "pch.h"
#include "MotionHandle.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	bool MotionHandle::isActive()
	{
		return MotionStorageManager::isActive(*this);
	}

	void MotionHandle::complete()
	{
		MotionStorageManager::completeMotion(*this);
	}

	void MotionHandle::cancel()
	{
		MotionStorageManager::cancelMotion(*this);
	}

	bool MotionHandle::operator == (const MotionHandle& other)
	{
		return Index == other.Index && Version == other.Version && StorageId == other.StorageId;
	}

	bool MotionHandle::operator !=(const MotionHandle& other)
	{
		return !(operator==(other));
	}
}
