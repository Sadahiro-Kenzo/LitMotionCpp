#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include "EaseUtility.h"
#include "MotionStorage.h"
#include "IMotionScheduler.h"
#include "MotionAdapter.h"

namespace LitMotionCpp
{
	class IUpdateRunner
	{
	public:
		virtual void update(double, double, double) = 0;
		virtual void reset() = 0;
	protected:
		static float getEasedValue(MotionDataCore& data, float value)
		{
			return data.Ease == Ease::CustomAnimationCurve ? value : EaseUtility::evaluate(value, data.Ease);
		}
	};

	template<typename TValue>
	class UpdateRunner :public IUpdateRunner
	{
	private:
		std::weak_ptr<MotionStorage<TValue>> m_storage;
		double m_prevTime;
		double m_prevUnscaleTime;
		double m_prevRealtime;
	public:
		UpdateRunner(std::weak_ptr<MotionStorage<TValue>> storage, double time, double unscaledTime, double realTime)
			:m_storage(storage)
			,m_prevTime(time)
			,m_prevUnscaleTime(unscaledTime)
			,m_prevRealtime(realTime)
		{

		}

		virtual void update(double time, double unscaledTime, double realTime) override
		{
			//	update data
			const auto DeltaTime = time - m_prevTime;
			const auto UnscaledDeltaTime = unscaledTime - m_prevUnscaleTime;
			const auto RealDeltaTime = realTime - m_prevRealtime;
			m_prevTime = time;
			m_prevUnscaleTime = unscaledTime;
			m_prevRealtime = realTime;

			auto storage = m_storage.lock();
			auto dataSpan = storage->getDataSpan();

			std::vector<TValue> output;
			output.reserve(dataSpan.size());
			std::vector<int> completedIndex;
			completedIndex.reserve(dataSpan.size());

			for (int index=0; index<dataSpan.size(); index++)
			{
				MotionData<TValue>& motionData = dataSpan[index];

				if (motionData.Core.Status == MotionStatus::Scheduled || motionData.Core.Status == MotionStatus::Delayed || motionData.Core.Status == MotionStatus::Playing) [[likely]]
				{
					double deltaTime = 0;
					switch (motionData.Core.TimeKind)
					{
					case MotionTimeKind::Time: deltaTime = DeltaTime; break;
					case MotionTimeKind::UnscaledTime:deltaTime = UnscaledDeltaTime; break;
					case MotionTimeKind::Realtime:deltaTime = RealDeltaTime; break;
					default:assert(false); break;
					}

					motionData.Core.Time = std::max(motionData.Core.Time + deltaTime * motionData.Core.PlaybackSpeed, 0.0);
					auto motionTime = motionData.Core.Time;

					double t;
					bool isCompleted;
					bool isDelayed;
					int completedLoops;
					int clampedCompletedLoops;

					if (motionData.Core.Duration <= 0.0) [[unlikely]]
					{
						if (motionData.Core.DelayType == DelayType::FirstLoop || motionData.Core.Delay == 0.0)
						{
							auto time = motionTime - motionData.Core.Delay;
							isCompleted = motionData.Core.Loops >= 0 && time > 0.0;
							if (isCompleted)
							{
								t = 1.0;
								completedLoops = motionData.Core.Loops;
							}
							else
							{
								t = 0.0;
								completedLoops = time < 0.0 ? -1 : 0;
							}
							clampedCompletedLoops = motionData.Core.Loops < 0 ? std::max(0, completedLoops) : std::clamp(completedLoops, 0, motionData.Core.Loops);
							isDelayed = time < 0;
						}
						else
						{
							completedLoops = static_cast<int>(std::floor(motionTime / motionData.Core.Delay));
							clampedCompletedLoops = motionData.Core.Loops < 0 ? std::max(0, completedLoops) : std::clamp(completedLoops, 0, motionData.Core.Loops);
							isDelayed = !isCompleted;
							t = isCompleted ? 1.0 : 0.0;
						}
					}
					else
					{
						if (motionData.Core.DelayType == DelayType::FirstLoop)
						{
							auto time = motionTime - motionData.Core.Delay;
							completedLoops = static_cast<int>(std::floor(time / motionData.Core.Duration));
							clampedCompletedLoops = motionData.Core.Loops < 0 ? std::max(0, completedLoops) : std::clamp(completedLoops, 0, motionData.Core.Loops);
							isCompleted = motionData.Core.Loops >= 0 && clampedCompletedLoops > motionData.Core.Loops - 1;
							isDelayed = time < 0;

							if (isCompleted)
							{
								t = 1.0;
							}
							else
							{
								auto currentLoopTime = time - motionData.Core.Duration * clampedCompletedLoops;
								t=std::clamp(currentLoopTime/motionData.Core.Duration,0.0,1.0);
							}
						}
						else
						{
							auto currentLoopTime = std::fmod(motionTime, motionData.Core.Duration + motionData.Core.Delay) - motionData.Core.Delay;
							completedLoops = static_cast<int>(std::floor(motionTime / (motionData.Core.Duration+ motionData.Core.Delay)));
							clampedCompletedLoops = motionData.Core.Loops < 0 ? std::max(0, completedLoops) : std::clamp(completedLoops, 0, motionData.Core.Loops);
							isCompleted = motionData.Core.Loops >= 0 && clampedCompletedLoops > motionData.Core.Loops - 1;
							isDelayed = currentLoopTime < 0;

							if (isCompleted)
							{
								t = 1.0;
							}
							else
							{
								t = std::clamp(currentLoopTime / motionData.Core.Duration, 0.0, 1.0);
							}
						}
					}

					float progress;
					switch (motionData.Core.LoopType)
					{
					case LoopType::Restart:
					{
						progress = getEasedValue(motionData.Core, static_cast<float>(t));
						break;
					}
					case LoopType::Yoyo:
					{
						progress = getEasedValue(motionData.Core, static_cast<float>(t));
						if ((clampedCompletedLoops + static_cast<int>(t)) % 2 == 1)
						{
							progress = 1.0f - progress;
						}
						break;
					}
					case LoopType::Incremental:
					{
						progress = getEasedValue(motionData.Core, 1.0f) * clampedCompletedLoops + getEasedValue(motionData.Core, static_cast<float>(std::fmod(t, 1.0)));
						break;
					}
					default:assert(false); break;
					}

					auto totalDuration = motionData.Core.DelayType == DelayType::FirstLoop
						? motionData.Core.Delay + motionData.Core.Duration * motionData.Core.Loops
						: (motionData.Core.Delay + motionData.Core.Duration) * motionData.Core.Loops;

					if (motionData.Core.Loops > 0 && motionTime >= totalDuration)
					{
						motionData.Core.Status = MotionStatus::Completed;
					}
					else if (isDelayed)
					{
						motionData.Core.Status = MotionStatus::Delayed;
					}
					else
					{
						motionData.Core.Status = MotionStatus::Playing;
					}

					MotionEvaluationContext context{progress};

					output.emplace_back(evaluate<TValue>(motionData.StartValue,motionData.EndValue,context));
				}
				else if(motionData.Core.Status==MotionStatus::Completed || motionData.Core.Status==MotionStatus::Canceled)
				{
					output.emplace_back();
					completedIndex.emplace_back(index);
					motionData.Core.Status = MotionStatus::Disposed;
				}
			}

			//	invoke delegates
			auto callbackSpan = storage->getCallbacksSpan();
			for (int index = 0; index < callbackSpan.size(); index++)
			{
				MotionData<TValue>& motionData = dataSpan[index];
				MotionCallbackData<TValue>& callbackData = callbackSpan[index];

				auto status = motionData.Core.Status;
				if (status == MotionStatus::Playing || (status == MotionStatus::Delayed && !callbackData.SkipValuesDuringDelay))
				{
					callbackData.UpdateAction(output[index]);
				}
				else if (status == MotionStatus::Completed)
				{
					callbackData.UpdateAction(output[index]);

					if (callbackData.OnCompleteAction)
					{
						callbackData.OnCancelAction();
					}
				}
			}

			storage->removeAll(completedIndex);
		}

		virtual void reset() override
		{
			m_storage.lock()->reset();
		}
	};

}//namespace