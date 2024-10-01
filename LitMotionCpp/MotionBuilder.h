#pragma once
#include <cassert>
#include <functional>
#include <memory>
#include "IMotionScheduler.h"
#include "MotionScheduler.h"
#include "MainLoopMotionScheduler.h"
#include "MotionData.h"
#include "MotionStatus.h"
#include "MotionCallbackData.h"
#include "PrimitiveMotionAdapter.h"
#include "EaseUtility.h"

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
		bool m_bindOnSchedule = false;
	public:
		MotionBuilder(const TValue& from, const TValue& to, float duration)
		{
			m_motionData.StartValue = from;
			m_motionData.EndValue = to;
			m_motionData.Core.Duration = duration;
		}
		/**
		* @brief Bind values when scheduling the motion.
		* 
		* @param [in] bindOnSchedule : Whether to bind on sheduling
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder& withBindOnSchedule(bool bindOnSchedule = true)
		{
			m_bindOnSchedule = bindOnSchedule;
			return *this;
		}
		MotionBuilder& withScheduler(std::weak_ptr<IMotionScheduler<TValue>> scheduler)
		{
			m_scheduler = scheduler;
			return *this;
		};
		MotionHandle bind(std::function<void(TValue&)> action)
		{
			setMotionData();
			setCallbackData(action);
			return schedule();
		};
		template<typename TState>
		MotionHandle bindWithState(TState* state, std::function<void(TValue&, TState*)> action)
		{
			setMotionData();
			setCallbackData<TState>(state,action);
			return schedule();
		}
	private:
		void setMotionData()
		{
			m_motionData.Core.Status = MotionStatus::Scheduled;
		}
		void setCallbackData(std::function<void(TValue&)> action)
		{
			m_callbackData.UpdateAction = action;
		}
		template<typename TState>
		void setCallbackData(TState* state,std::function<void(TValue&,TState*)> action)
		{
			m_callbackData.UpdateAction = [=](TValue& value)
				{
					action(value, state);
				};
		}
		MotionHandle schedule()
		{
			if (m_bindOnSchedule && m_callbackData.UpdateAction)
			{
				auto bindValue = evaluate<TValue>(
					m_motionData.StartValue,
					m_motionData.EndValue,
					MotionEvaluationContext{ EaseUtility::evaluate(0.0,m_motionData.Core.Ease) }
				);
				m_callbackData.UpdateAction(bindValue);
			}

			MotionHandle handle;

			if (m_scheduler.expired())
			{
				auto defaultScheduler = MotionScheduler::getDefault<TValue>();
				if (defaultScheduler.expired())
				{
					MotionScheduler::setDefault<TValue>(std::make_shared<MainLoopMotionScheduler<TValue>>(MotionTimeKind::Time));
					defaultScheduler = MotionScheduler::getDefault<TValue>();
				}
				handle = defaultScheduler.lock()->schedule(m_motionData, m_callbackData);
			}
			else
			{
				handle=m_scheduler.lock()->schedule(m_motionData, m_callbackData);
			}

			return handle;
		}
	};
}//namespace

