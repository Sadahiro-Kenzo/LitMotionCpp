#pragma once
#include <memory>
#include "FastListCore.h"
#include "MotionStorage.h"
#include "UpdateRunner.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	class Cache
	{
	private:
		friend class ManualMotionDispatcher;

		static std::shared_ptr<MotionStorage<TValue,TOptions, TAdapter>> updateStorage;
		static std::shared_ptr<UpdateRunner<TValue,TOptions, TAdapter>> updateRunner;

		static std::weak_ptr<MotionStorage<TValue,TOptions, TAdapter>> getOrCreate()
		{
			if (!updateStorage)
			{
				auto storage = std::make_shared<MotionStorage<TValue,TOptions, TAdapter>>(MotionStorageManager::getCurrentStorageId());
				MotionStorageManager::addStorage(storage);
				updateStorage = storage;
			}
			return updateStorage;
		}
	};
	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::shared_ptr<MotionStorage<TValue,TOptions,TAdapter>> Cache<TValue,TOptions,TAdapter>::updateStorage;
	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::shared_ptr<UpdateRunner<TValue,TOptions,TAdapter>> Cache<TValue,TOptions,TAdapter>::updateRunner;

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

		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData)
		{
			auto storage = Cache<TValue,TOptions,TAdapter>::getOrCreate();
			if (!Cache<TValue,TOptions,TAdapter>::updateRunner)
			{
				auto runner = std::make_shared<UpdateRunner<TValue,TOptions,TAdapter>>(storage, getTime(), getTime(), getTime());
				m_updateRunners.add(runner);
				Cache<TValue,TOptions,TAdapter>::updateRunner = runner;
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

