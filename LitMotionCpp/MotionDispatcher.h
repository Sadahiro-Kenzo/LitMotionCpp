#pragma once
#include <map>
#include <memory>
#include "FastListCore.h"
#include "MotionStorage.h"
#include "UpdateRunner.h"
#include "MotionStorageManager.h"

namespace LitMotionCpp
{
	class MotionScheduler;

	class MotionDispatcher
	{
	public:
		/*
		* @brief get time of beginning of this frame.
		*/
		static double getTime();

		/*
		* @brief get TimeScale-independent time for this frame.
		*/
		static double getUnscaledTime();

		/*
		* @brief get real time since startup.
		*/
		static double getRealtimeSinceStartup();

		/*
		* @brief get real time since startup.
		* 
		* must call,top of main loop.
		*/
		static void setRealtimeSinceStartup(double);

		/*
		* @brief get TimeScale which affects getTime() progression.
		*/
		static double getTimeScale();

		/*
		* @brief set TimeScale which affects getTime() progression.
		*/
		static void setTimeScale(double);

		/*
		* @brief get max delta time which affects getTime() progression.
		*/
		static double getMaxDeltaTime();

		/*
		* @brief set max delta time which affects getTime() progression.
		*/
		static void setMaxDeltaTime(double);

		/*
		* @brief initialize time values.
		*/
		static void initializeTime();
	private:
		static std::map<int, FastListCore<std::weak_ptr<IUpdateRunner>>> fastUpdateRunners;

		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		class StorageCache
		{
		private:
			static std::map<int, std::shared_ptr<MotionStorage<TValue,TOptions,TAdapter>>> updateStorages;
		public:
			static std::weak_ptr<MotionStorage<TValue,TOptions,TAdapter>> getOrCreate(int loopTiming)
			{
				if (updateStorages.contains(loopTiming))
				{
					return updateStorages[loopTiming];
				}
				else
				{
					auto storage = std::make_shared<MotionStorage<TValue,TOptions,TAdapter>>(MotionStorageManager::getCurrentStorageId());
					MotionStorageManager::addStorage(storage);
					updateStorages.insert(std::make_pair(loopTiming, storage));
					return storage;
				}
			}
		};

		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		class RunnerCache
		{
		private:
			static std::map<int, std::shared_ptr<UpdateRunner<TValue,TOptions,TAdapter>>> updateRunners;
		public:
			static std::weak_ptr<UpdateRunner<TValue,TOptions, TAdapter>> getOrCreate(int loopTiming,std::weak_ptr<MotionStorage<TValue,TOptions, TAdapter>> storage)
			{
				if (updateRunners.contains(loopTiming))
				{
					return updateRunners[loopTiming];
				}
				else
				{
					std::shared_ptr<UpdateRunner<TValue,TOptions, TAdapter>> runner = std::make_shared<UpdateRunner<TValue,TOptions, TAdapter>>(storage, getTime(),getUnscaledTime(),getRealtimeSinceStartup());
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

		template<typename TValue, typename TOptions, typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static MotionHandle schedule(const MotionData<TValue,TOptions>& data, const MotionCallbackData& callbackData,int looptiming)
		{
			auto storage = StorageCache<TValue,TOptions, TAdapter>::getOrCreate(looptiming);
			RunnerCache<TValue,TOptions, TAdapter>::getOrCreate(looptiming,storage);

			auto [entryIndex, version] = storage.lock()->append(data, callbackData);

			MotionHandle result;
			result.StorageId = storage.lock()->getStorageId();
			result.Index = entryIndex;
			result.Version = version;

			return result;
		}

		/**
		* @brief update motions. call in main loop.
		* 
		* Update all motions registered in MotionScheduler of same PlayerLoopTiming.
		* 
		* @param [in] scheduler : MotionScheduler to update.
		*/
		static void update(const MotionScheduler& scheduler);

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

	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::map<int, std::shared_ptr<MotionStorage<TValue,TOptions, TAdapter>>> MotionDispatcher::StorageCache<TValue,TOptions, TAdapter>::updateStorages;

	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::map<int, std::shared_ptr<UpdateRunner<TValue,TOptions, TAdapter>>> MotionDispatcher::RunnerCache<TValue,TOptions, TAdapter>::updateRunners;


}//namespace

