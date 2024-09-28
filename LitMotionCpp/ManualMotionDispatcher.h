#pragma once
#include <memory>
#include "FastListCore.h"
#include "MotionStorage.h"
#include "UpdateRunner.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	template<typename TValue>
	class Cache
	{
	private:
		friend class ManualMotionDispatcher;

		static std::shared_ptr<MotionStorage<TValue>> updateStorage;
		static std::shared_ptr<UpdateRunner<TValue>> updateRunner;

		static std::weak_ptr<MotionStorage<TValue>> getOrCreate()
		{
			if (!updateStorage)
			{
				auto storage = std::make_shared<MotionStorage<TValue>>(MotionStorageManager::getCurrentStorageId());
				MotionStorageManager::addStorage(storage);
				updateStorage = storage;
			}
			return updateStorage;
		}
	};
	template<typename TValue>
	std::shared_ptr<MotionStorage<TValue>> Cache<TValue>::updateStorage;
	template<typename TValue>
	std::shared_ptr<UpdateRunner<TValue>> Cache<TValue>::updateRunner;

	class ManualMotionDispatcher
	{
	private:
		static FastListCore<std::weak_ptr<IUpdateRunner>> m_updateRunners;
	public:
		static double getTime();
		static void setTime(double);

		/**
		* @brief Update all scheduled motions with MotionScheduler.Manual
		*
		* @param [in] deltaTime
		*/
		static void update(double deltaTime);
		/**
		* @brief Update all scheduled motions with MotionScheduler.Manual
		*/
		static void update();
		/**
		* @brief Cancel all motions and reset data.
		*/
		static void reset();

		template<typename TValue>
		static MotionHandle schedule(const MotionData<TValue>& data, const MotionCallbackData<TValue>& callbackData)
		{
			auto storage = Cache<TValue>::getOrCreate();
			if (!Cache<TValue>::updateRunner)
			{
				auto runner = std::make_shared<UpdateRunner<TValue>>(storage, getTime(), getTime(), getTime());
				m_updateRunners.add(runner);
				Cache<TValue>::updateRunner = runner;
			}

			auto [entryIndex, version] = storage.lock()->append(data, callbackData);
			
			MotionHandle result;
			result.StorageId = storage.lock()->getStorageId();
			result.Index = entryIndex;
			result.Version = version;

			return result;
		}
	};
}//	namespace

