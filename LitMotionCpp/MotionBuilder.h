#pragma once
#include <functional>
#include <memory>
#include "IMotionScheduler.h"
#include "MotionData.h"
#include "MotionStatus.h"
#include "MotionCallbackData.h"

namespace LitMotionCpp
{
	template<typename TValue>
	class MotionBuilder
	{
	private:
		std::weak_ptr<IMotionScheduler<TValue>> m_scheduler;
		std::function<void(TValue)> m_updateAction;
		MotionData<TValue> m_motionData;
		MotionCallbackData<TValue> m_callbackData;
	public:
		MotionBuilder(TValue from, TValue to, TValue duration)
		{
			m_motionData.StartValue = from;
			m_motionData.EndValue = to;
			m_motionData.Core.Duration = duration;
		}
		MotionBuilder& withScheduler(std::weak_ptr<IMotionScheduler<TValue>> scheduler)
		{
			m_scheduler = scheduler;
			return *this;
		};
		MotionHandle bind(std::function<void(TValue)> action)
		{
			SetMotionData();
			SetCallbackData(action);
			return m_scheduler.lock()->schedule(m_motionData,m_callbackData);
		};
	private:
		void SetMotionData()
		{
			m_motionData.Core.Status = MotionStatus::Scheduled;
		}
		void SetCallbackData(std::function<void(TValue)> action)
		{
			m_callbackData.UpdateAction = action;
		}
	};
}//namespace

