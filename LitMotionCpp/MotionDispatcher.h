#pragma once
#include <map>
#include <memory>
#include "FastListCore.h"
#include "MotionStorage.h"
#include "UpdateRunner.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	class MotionDispatcher
	{
	public:
		/*
		* @brief get time of beginning of this frame.
		*/
		static double getTime();

		/*
		* @brief set time of beginning of this frame.
		* 
		* must call,top of main loop.
		*/
		static void setTime(double);
	private:
		static std::map<int, FastListCore<std::weak_ptr<IUpdateRunner>>> fastUpdateRunners;

		template<typename TValue>
		class StorageCache
		{
		private:
			static std::map<int, std::shared_ptr<MotionStorage<TValue>>> updateStorages;
		public:
			static std::weak_ptr<MotionStorage<TValue>> getOrCreate(int loopTiming)
			{
				if (updateStorages.contains(loopTiming))
				{
					return updateStorages[loopTiming];
				}
				else
				{
					auto storage = std::make_shared<MotionStorage<TValue>>(MotionStorageManager::getCurrentStorageId());
					MotionStorageManager::addStorage(storage);
					updateStorages.insert(std::make_pair(loopTiming, storage));
					return storage;
				}
			}
		};

		template<typename TValue>
		class RunnerCache
		{
		private:
			static std::map<int, std::shared_ptr<UpdateRunner<TValue>>> updateRunners;
		public:
			static std::weak_ptr<UpdateRunner<TValue>> getOrCreate(int loopTiming,std::weak_ptr<MotionStorage<TValue>> storage)
			{
				if (updateRunners.contains(loopTiming))
				{
					return updateRunners[loopTiming];
				}
				else
				{
					std::shared_ptr<UpdateRunner<TValue>> runner = std::make_shared<UpdateRunner<TValue>>(storage, getTime(), getTime(), getTime());
					updateRunners.insert(std::make_pair(loopTiming, runner));

					if (!fastUpdateRunners.contains(loopTiming))
					{
						fastUpdateRunners.emplace(loopTiming, FastListCore<std::weak_ptr<IUpdateRunner>>{});
					}
					fastUpdateRunners[loopTiming].add(runner);

					return runner;
				}
			}
		};
	public:
		/**
		* Cancel all motions.
		*/
		static void clear();

		template<typename TValue>
		static MotionHandle schedule(const MotionData<TValue>& data, const MotionCallbackData& callbackData,int looptiming)
		{
			auto storage = StorageCache<TValue>::getOrCreate(looptiming);
			RunnerCache<TValue>::getOrCreate(looptiming,storage);

			auto [entryIndex, version] = storage.lock()->append(data, callbackData);

			MotionHandle result;
			result.StorageId = storage.lock()->getStorageId();
			result.Index = entryIndex;
			result.Version = version;

			return result;
		}

		/**
		* @brief update motions. call in main loop.
		*/
		static void update();
	};

	template<typename TValue>
	std::map<int, std::shared_ptr<MotionStorage<TValue>>> MotionDispatcher::StorageCache<TValue>::updateStorages;

	template<typename TValue>
	std::map<int, std::shared_ptr<UpdateRunner<TValue>>> MotionDispatcher::RunnerCache<TValue>::updateRunners;


}//namespace

