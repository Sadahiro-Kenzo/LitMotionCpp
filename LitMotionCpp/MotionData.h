#pragma once
#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include "AnimationCurve.h"
#include "EaseUtility.h"
#include "IMotionOptions.h"
#include "IMotionAdapter.h"
#include "MotionStatus.h"

namespace LitMotionCpp
{
	enum class MotionTimeKind
	{
		Time,
		UnscaledTime,
		Realtime,
	};

	struct MotionDataCore
	{
		struct MotionState
		{
			MotionStatus Status = MotionStatus::None;
			MotionStatus PrevStatus = MotionStatus::None;
			bool IsPreserved = false;
			bool IsInSequence = false;

			std::uint16_t CompletedLoops;
			std::uint16_t PrevCompletedLoops;

			double Time = 0.0f;
			float PlaybackSpeed = 1.0f;

			bool wasStatusChanged() { return Status != PrevStatus; }
			bool wasLoopCompleted() { return CompletedLoops > PrevCompletedLoops; }
		};

		struct MotionParameters
		{
			float Duration = 0.0f;
			Ease Ease = Ease::Linear;
			IAnimationCurve* AnimationCurve = nullptr;
			MotionTimeKind TimeKind = MotionTimeKind::Time;
			float Delay = 0.0f;
			int Loops = 1;
			DelayType DelayType = DelayType::FirstLoop;
			LoopType LoopType = LoopType::Restart;

			double getTotalDuration()
			{
				if (Loops < 0)
				{
					return std::numeric_limits<double>::infinity();
				}

				return Delay * (DelayType == DelayType::EveryLoop ? Loops : 1) + Duration * Loops;
			}

			MotionParameters(float duration):Duration(duration){}
		};

		MotionState State;
		MotionParameters Parameters;

		MotionDataCore(float duration) :Parameters(duration) {}

		double getTimeSinceStart() { return State.Time - Parameters.Delay; }

		int getClampedCompletedLoops(int completedLoops)
		{
			return Parameters.Loops < 0 ? std::max(0, completedLoops) : std::clamp(completedLoops, 0, Parameters.Loops);
		}

		float getEasedValue(float value)
		{
			if (Parameters.Ease == Ease::CustomAnimationCurve)
			{
				return Parameters.AnimationCurve->evaluate(value);
			}
			return EaseUtility::evaluate(value, Parameters.Ease);
		}

		void update(double time, float& progress)
		{
			State.PrevCompletedLoops = State.CompletedLoops;
			State.PrevStatus = State.Status;

			State.Time = time;
			time = std::max(time, 0.0);

			double t;
			bool isCompleted;
			bool isDelayed;
			int completedLoops;
			int clampedCompletedLoops;

			if (Parameters.Duration <= 0.0f) [[unlikely]]
			{
				if (Parameters.DelayType == DelayType::FirstLoop || Parameters.Delay == 0.0f)
				{
					isCompleted = Parameters.Loops >= 0 && getTimeSinceStart() > 0.0f;
					if (isCompleted)
					{
						t = 1.0f;
						completedLoops = Parameters.Loops;
					}
					else
					{
						t = 0.0f;
						completedLoops = getTimeSinceStart() < 0.0f ? -1 : 0;
					}
					clampedCompletedLoops = getClampedCompletedLoops(completedLoops);
					isDelayed = getTimeSinceStart() < 0;
				}
				else
				{
					completedLoops = static_cast<int>(std::floor(time / Parameters.Delay));
					clampedCompletedLoops = getClampedCompletedLoops(completedLoops);
					isCompleted = Parameters.Loops >= 0 && clampedCompletedLoops > Parameters.Loops - 1;
					isDelayed = !isCompleted;
					t = isCompleted ? 1.0f : 0.0f;
				}
			}
			else
			{
				if (Parameters.DelayType == DelayType::FirstLoop)
				{
					completedLoops = static_cast<int>(std::floor(getTimeSinceStart() / Parameters.Duration));
					clampedCompletedLoops = getClampedCompletedLoops(completedLoops);
					isCompleted = Parameters.Loops >= 0 && clampedCompletedLoops > Parameters.Loops - 1;
					isDelayed = getTimeSinceStart() < 0.0f;

					if (isCompleted)
					{
						t = 1.0f;
					}
					else
					{
						auto currentLoopTime = getTimeSinceStart() - Parameters.Duration * clampedCompletedLoops;
						t = std::clamp(currentLoopTime / Parameters.Duration, 0.0, 1.0);
					}
				}
				else
				{
					auto currentLoopTime = std::fmod(time, Parameters.Duration + Parameters.Delay) - Parameters.Delay;
					completedLoops = static_cast<int>(std::floor(time/(Parameters.Duration + Parameters.Delay)));
					clampedCompletedLoops = getClampedCompletedLoops(completedLoops);
					isCompleted = Parameters.Loops >= 0 && clampedCompletedLoops > Parameters.Loops - 1;
					isDelayed = currentLoopTime < 0;

					if (isCompleted)
					{
						t = 1.0f;
					}
					else
					{
						t = std::clamp(currentLoopTime / Parameters.Duration, 0.0, 1.0);
					}
				}
			}

			State.CompletedLoops = static_cast<std::uint16_t>(clampedCompletedLoops);

			switch(Parameters.LoopType)
			{
			default:
			case LoopType::Restart:
				progress = getEasedValue(static_cast<float>(t));
				break;
			case LoopType::Flip:
				progress = getEasedValue(static_cast<float>(t));
				if ((clampedCompletedLoops + static_cast<int>(t)) % 2 == 1)
				{
					progress = 1.0f - progress;
				}
				break;
			case LoopType::Incremental:
				progress = getEasedValue(1.0f) * clampedCompletedLoops + getEasedValue(static_cast<float>(std::fmod(t, 1.0f)));
				break;
			case LoopType::Yoyo:
				progress = (clampedCompletedLoops + static_cast<int>(t)) % 2 == 1
					? getEasedValue(static_cast<float>(1.0f - t))
					: getEasedValue(static_cast<float>(t));
				break;
			}

			if (isCompleted)
			{
				State.Status = MotionStatus::Completed;
			}
			else if (isDelayed || State.Time < 0)
			{
				State.Status = MotionStatus::Delayed;
			}
			else
			{
				State.Status = MotionStatus::Playing;
			}
		}

		void complete(float& progress)
		{
			State.Status = MotionStatus::Completed;
			State.Time = Parameters.getTotalDuration();
			State.CompletedLoops = static_cast<std::uint16_t>(Parameters.Loops);

			switch (Parameters.LoopType)
			{
			default:
			case LoopType::Restart:progress = 1.0f;break;
			case LoopType::Flip:
			case LoopType::Yoyo:progress = Parameters.Loops % 2 == 0 ? 0.0f : 1.0f; break;
			case LoopType::Incremental:progress = static_cast<float>(Parameters.Loops);break;
			}
		}
	};

	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	struct MotionData
	{
		MotionDataCore Core;
		TValue StartValue;
		TValue EndValue;
		TOptions Options;

		MotionData(const TValue& startValue, const TValue& endValue,const float duration)
			:Core{duration}
			, StartValue{ startValue }
			, EndValue{ endValue }
		{

		}

		MotionData()
			:Core{0.0f}
			,StartValue{}
			,EndValue{}
		{

		}

		template<typename TAdapter>
			requires std::derived_from<TAdapter,IMotionAdapter<TValue,TOptions>>
		void update(double time, TValue& result)
		{
			float progress;
			Core.update(time, progress);

			MotionEvaluationContext context{
				.Progress = progress,
				.Time = time
			};
			TAdapter adapter;
			result = adapter.evaluate(StartValue, EndValue, Options, context);
		}

		template<typename TAdapter>
			requires std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		void complete(TValue& result)
		{
			float progress;
			Core.complete(progress);

			MotionEvaluationContext context{
				.Progress = progress,
				.Time = Core.State.Time
			};
			TAdapter adapter;
			result = adapter.evaluate(StartValue, EndValue, Options, context);
		}
	};

}//namespace

