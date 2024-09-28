#include "pch.h"
#include <cassert>
#include "MotionStorageManager.h"
#include "MotionStorage.h"
#include "MotionData.h"
#include "MotionCallbackData.h"
#include "MotionHandle.h"
#include "FastListCore.h"

namespace LitMotionCpp
{
	static FastListCore<std::weak_ptr<IMotionStorage>> s_storageList;
	static int s_currentStorageId = 0;

	static void checkStorageId(const MotionHandle& handle)
	{
		assert(handle.StorageId >= 0 && handle.StorageId < s_currentStorageId);
	}

	int MotionStorageManager::getCurrentStorageId()
	{
		return s_currentStorageId;
	}

	void MotionStorageManager::addStorage(std::weak_ptr<IMotionStorage> storage)
	{
		s_storageList.add(storage);
		s_currentStorageId++;
	}

	void MotionStorageManager::completeMotion(const MotionHandle& handle)
	{
		checkStorageId(handle);
		s_storageList[handle.StorageId].lock()->complete(handle);
	}

	void MotionStorageManager::cancelMotion(const MotionHandle& handle)
	{
		checkStorageId(handle);
		s_storageList[handle.StorageId].lock()->cancel(handle);
	}

	bool MotionStorageManager::isActive(const MotionHandle& handle)
	{
		if (handle.StorageId < 0 || handle.StorageId >= s_currentStorageId)
		{
			return false;
		}

		return s_storageList[handle.StorageId].lock()->isActive(handle);
	}

	MotionDataCore& MotionStorageManager::getMotionDataRef(const MotionHandle& handle)
	{
		checkStorageId(handle);
		return s_storageList[handle.StorageId].lock()->getDataRef(handle);
	}

	MotionCallbackDataCore& MotionStorageManager::getMotionCallbackDataRef(const MotionHandle& handle)
	{
		checkStorageId(handle);
		return s_storageList[handle.StorageId].lock()->getCallbackDataRef(handle);
	}

}//namespace