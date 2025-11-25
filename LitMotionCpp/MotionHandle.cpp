#include "pch.h"
#include "MotionHandle.h"
#include "MotionStorageManager.h"
#include "MotionData.h"
#include "Error.h"

namespace LitMotionCpp
{
	void MotionHandle::setPlaybackSpeed(float value)
	{
#ifdef _DEBUG
		if (value < 0.0f)
		{
			Error::playbackSpeedMustBeZeroOrGreater();
		}
#endif
		MotionStorageManager::getMotionDataRef(*this).State.PlaybackSpeed = value;
	}

	float MotionHandle::getPlaybackSpeed()
	{
		return MotionStorageManager::getMotionDataRef(*this).State.PlaybackSpeed;
	}

	bool MotionHandle::isActive() const
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
