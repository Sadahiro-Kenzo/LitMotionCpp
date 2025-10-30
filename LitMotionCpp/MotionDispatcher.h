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

		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		class StorageCache
		{
		private:
			static std::map<int, std::shared_ptr<MotionStorage<TValue,TOptions>>> updateStorages;
		public:
			static std::weak_ptr<MotionStorage<TValue,TOptions>> getOrCreate(int loopTiming)
			{
				if (updateStorages.contains(loopTiming))
				{
					return updateStorages[loopTiming];
				}
				else
				{
					auto storage = std::make_shared<MotionStorage<TValue,TOptions>>(MotionStorageManager::getCurrentStorageId());
					MotionStorageManager::addStorage(storage);
					updateStorages.insert(std::make_pair(loopTiming, storage));
					return storage;
				}
			}
		};

		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		class RunnerCache
		{
		private:
			static std::map<int, std::shared_ptr<UpdateRunner<TValue,TOptions>>> updateRunners;
		public:
			static std::weak_ptr<UpdateRunner<TValue,TOptions>> getOrCreate(int loopTiming,std::weak_ptr<MotionStorage<TValue,TOptions>> storage)
			{
				if (updateRunners.contains(loopTiming))
				{
					return updateRunners[loopTiming];
				}
				else
				{
					std::shared_ptr<UpdateRunner<TValue,TOptions>> runner = std::make_shared<UpdateRunner<TValue,TOptions>>(storage, getTime(), getTime(), getTime());
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

		template<typename TValue, typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData,int looptiming)
		{
			auto storage = StorageCache<TValue,TOptions>::getOrCreate(looptiming);
			RunnerCache<TValue,TOptions>::getOrCreate(looptiming,storage);

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

		/*@
		* @brief get last exception message.
		*/
		static const char* getLastError();

		/**
		* @brief Set handling of unhandled exceptions. 
		*/
		static void registerUnhandledExceptionHandler(std::function<void(std::exception&)> unhandledExceptionHandler);

		/**
		* @brief  Get handling of unhandled exceptions.
		*/
		static std::function<void(std::exception&)> getUnhandledExceptionHandler();
	};

	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::map<int, std::shared_ptr<MotionStorage<TValue,TOptions>>> MotionDispatcher::StorageCache<TValue,TOptions>::updateStorages;

	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::map<int, std::shared_ptr<UpdateRunner<TValue,TOptions>>> MotionDispatcher::RunnerCache<TValue,TOptions>::updateRunners;


}//namespace

