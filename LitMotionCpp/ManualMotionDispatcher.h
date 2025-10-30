#pragma once
#include <memory>
#include "FastListCore.h"
#include "MotionStorage.h"
#include "UpdateRunner.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	class Cache
	{
	private:
		friend class ManualMotionDispatcher;

		static std::shared_ptr<MotionStorage<TValue,TOptions>> updateStorage;
		static std::shared_ptr<UpdateRunner<TValue,TOptions>> updateRunner;

		static std::weak_ptr<MotionStorage<TValue,TOptions>> getOrCreate()
		{
			if (!updateStorage)
			{
				auto storage = std::make_shared<MotionStorage<TValue,TOptions>>(MotionStorageManager::getCurrentStorageId());
				MotionStorageManager::addStorage(storage);
				updateStorage = storage;
			}
			return updateStorage;
		}
	};
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::shared_ptr<MotionStorage<TValue,TOptions>> Cache<TValue,TOptions>::updateStorage;
	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::shared_ptr<UpdateRunner<TValue,TOptions>> Cache<TValue,TOptions>::updateRunner;

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

		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData)
		{
			auto storage = Cache<TValue,TOptions>::getOrCreate();
			if (!Cache<TValue,TOptions>::updateRunner)
			{
				auto runner = std::make_shared<UpdateRunner<TValue,TOptions>>(storage, getTime(), getTime(), getTime());
				m_updateRunners.add(runner);
				Cache<TValue,TOptions>::updateRunner = runner;
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

