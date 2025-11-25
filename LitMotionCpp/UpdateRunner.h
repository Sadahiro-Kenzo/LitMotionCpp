#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include "MotionStorage.h"
#include "IMotionScheduler.h"
#include "IMotionAdapter.h"
#include "MotionUpdateJob.h"
#include "ParallelJobScheduler.h"

namespace LitMotionCpp
{
	extern void invokeUnhandledExceptionHandler(std::exception&);

	class IUpdateRunner
	{
	public:
		virtual void update(double, double, double) = 0;
		virtual void reset() = 0;
	};

	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	class UpdateRunner :public IUpdateRunner
	{
	private:
		std::weak_ptr<MotionStorage<TValue,TOptions,TAdapter>> m_storage;
		double m_prevTime;
		double m_prevUnscaleTime;
		double m_prevRealtime;
	public:
		UpdateRunner(std::weak_ptr<MotionStorage<TValue,TOptions,TAdapter>> storage, double time, double unscaledTime, double realTime)
			:m_storage(storage)
			,m_prevTime(time)
			,m_prevUnscaleTime(unscaledTime)
			,m_prevRealtime(realTime)
		{

		}

		virtual void update(double timeParameter, double unscaledTime, double realTime) override
		{
			//	update data
			const auto DeltaTime = timeParameter - m_prevTime;
			const auto UnscaledDeltaTime = unscaledTime - m_prevUnscaleTime;
			const auto RealDeltaTime = realTime - m_prevRealtime;
			m_prevTime = timeParameter;
			m_prevUnscaleTime = unscaledTime;
			m_prevRealtime = realTime;

			auto storage = m_storage.lock();
			auto dataSpan = storage->getDataSpan();

			std::vector<TValue> output;
			output.resize(dataSpan.size());
			std::vector<int> completedIndex;
			completedIndex.reserve(dataSpan.size());

			MotionUpdateJob<TValue, TOptions, TAdapter> job({
				.DataSpan = dataSpan,
				.Output = output,
				.CompletedIndex = completedIndex,
				.DeltaTime = DeltaTime,
				.UnscaledDeltaTime = UnscaledDeltaTime,
				.RealDeltaTime = RealDeltaTime
				});
			ParallelJobScheduler::execute(job, dataSpan.size(), 16);

			//	invoke delegates
			auto callbackSpan = storage->getCallbacksSpan();
			for (int index = 0; index < callbackSpan.size(); index++)
			{
				MotionData<TValue,TOptions>& motionData = dataSpan[index];
				auto& state = motionData.Core.State;
				MotionCallbackData& callbackData = callbackSpan[index];

				auto status = state.Status;
#ifdef LIT_MOTION_CPP_ENABLE_EXCEPTION
				if (status == MotionStatus::Playing || (status == MotionStatus::Delayed && !callbackData.SkipValuesDuringDelay))
				{
					try
					{
						callbackData.invoke(output[index]);
					}
					catch(std::exception& exception)
					{
						invokeUnhandledExceptionHandler(exception);
						if (callbackData.CancelOnError)
						{
							status = MotionStatus::Canceled;
							if (callbackData.OnCancelAction)
							{
								callbackData.OnCancelAction();
							}
						}
					}
				}
				else if (status == MotionStatus::Completed)
				{
					try
					{
						callbackData.invoke(output[index]);
					}
					catch(std::exception& exception)
					{
						invokeUnhandledExceptionHandler(exception);
						if (callbackData.CancelOnError)
						{
							status = MotionStatus::Canceled;
							if (callbackData.OnCancelAction)
							{
								callbackData.OnCancelAction();
							}
							continue;
						}
					}

					if (callbackData.OnCompleteAction)
					{
						try
						{
							callbackData.OnCompleteAction();
						}
						catch (std::exception& exception)
						{
							invokeUnhandledExceptionHandler(exception);
						}
					}
				}
#else
				if (status == MotionStatus::Playing || (status == MotionStatus::Delayed && !callbackData.SkipValuesDuringDelay))
				{
					callbackData.invoke(output[index]);
				}
				else if (status == MotionStatus::Completed)
				{
					callbackData.invoke(output[index]);

					if (callbackData.OnCompleteAction)
					{
						callbackData.OnCompleteAction();
					}
				}
#endif
			}

			storage->removeAll(completedIndex);
		}

		virtual void reset() override
		{
			m_storage.lock()->reset();
		}
	};

}//namespace