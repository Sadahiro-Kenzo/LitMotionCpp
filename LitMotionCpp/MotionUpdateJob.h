#pragma once
#include <cassert>
#include <mutex>
#include <span>
#include <vector>
#include "IJobParallelFor.h"
#include "IMotionAdapter.h"
#include "IMotionOptions.h"
#include "MotionData.h"
#include "MotionStatus.h"

namespace LitMotionCpp
{
	template<typename TValue, typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	struct MotionUpdateJobSpec
	{
		std::span<MotionData<TValue, TOptions>> DataSpan;
		std::vector<TValue>& Output;
		std::vector<int>& CompletedIndex;
		double DeltaTime;
		double UnscaledDeltaTime;
		double RealDeltaTime;
	};


	template<typename TValue, typename TOptions, typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	struct MotionUpdateJob :public IJobParallelFor
	{
		std::span<MotionData<TValue, TOptions>> DataSpan;
		std::vector<TValue>& Output;
		std::vector<int>& CompletedIndex;
		std::mutex CompletedIndexMutex;
		const double DeltaTime;
		const double UnscaledDeltaTime;
		const double RealDeltaTime;

		MotionUpdateJob() = delete;
		MotionUpdateJob(const MotionUpdateJobSpec<TValue,TOptions>& spec)
			:DataSpan(spec.DataSpan)
			,Output(spec.Output)
			,CompletedIndex(spec.CompletedIndex)
			,DeltaTime(spec.DeltaTime)
			,UnscaledDeltaTime(spec.UnscaledDeltaTime)
			,RealDeltaTime(spec.RealDeltaTime)
		{
			assert(DataSpan.size() <= Output.size());
			assert(DataSpan.size() <= CompletedIndex.capacity());
		}

		virtual void execute(int index) override
		{
			MotionData<TValue, TOptions>& motionData = DataSpan[index];
			auto& state = motionData.Core.State;
			auto& parameters = motionData.Core.Parameters;

			if ((state.Status == MotionStatus::Scheduled || state.Status == MotionStatus::Delayed || state.Status == MotionStatus::Playing) ||
				(state.IsPreserved && state.Status==MotionStatus::Completed))
			{
				if (state.IsInSequence) [[unlikely]]
				{
					return;
				}

				double deltaTime = 0;
				switch (parameters.TimeKind)
				{
				case MotionTimeKind::Time: deltaTime = DeltaTime; break;
				case MotionTimeKind::UnscaledTime:deltaTime = UnscaledDeltaTime; break;
				case MotionTimeKind::Realtime:deltaTime = RealDeltaTime; break;
				default:assert(false); break;
				}

				auto time = state.Time + deltaTime * state.PlaybackSpeed;
				motionData.update<TAdapter>(time, Output[index]);
			}
			else if ((!state.IsPreserved && state.Status == MotionStatus::Completed) || state.Status == MotionStatus::Canceled)
			{
				std::lock_guard<std::mutex> lock(CompletedIndexMutex);

				CompletedIndex.emplace_back(index);
				state.Status = MotionStatus::Disposed;
			}
		}
	};

}//namespace LitMotionCpp