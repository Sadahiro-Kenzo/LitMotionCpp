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
	/**
	* @brief Supports construction, scheduling, and binding of motion entities.
	* 
	* @tparam TValue : The type of value to animate
	*/
	template<typename TValue>
	class MotionBuilder
	{
	private:
		std::weak_ptr<IMotionScheduler<TValue>> m_scheduler;
		std::function<void(TValue)> m_updateAction;
		MotionData<TValue> m_motionData;
		MotionCallbackData m_callbackData;
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

		/**
		* @brief Specifies the scheduler that schedule the motion.
		* 
		* @param [in] scheduler : Scheduler
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder& withScheduler(std::weak_ptr<IMotionScheduler<TValue>> scheduler)
		{
			m_scheduler = scheduler;
			return *this;
		};

		/**
		* @brief Create motion and bind it to a specific object, property, etc.
		* 
		* @param[in] action : Action that handles binding
		* 
		* @return Handle of the created motion data.
		*/
		MotionHandle bind(std::function<void(TValue&)> action)
		{
			setMotionData();
			setCallbackData(action);
			return schedule();
		};

		/**
		* @brief Create motion and bind it to a specific object. Unlike the regular Bind method, it avoids allocation by closure by passing an object.
		* 
		* @tparam TState : Type of state
		* @param [in] state : Motion state
		* @param[in] action : Action that handles binding
		* 
		* @return Handle of the created motion data.
		*/
		template<typename TState>
		MotionHandle bindWithState(TState* state, std::function<void(TValue&, TState*)> action)
		{
			setMotionData();
			setCallbackData<TState>(state,action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object. Unlike the regular Bind method, it avoids allocation by closure by passing an object.
		*
		* @tparam TState1 : Type of state1
		* @tparam TState2 : Type of state2
		* @param [in] state1 : Motion state1
		* @param [in] state2 : Motion state2
		* @param[in] action : Action that handles binding
		*
		* @return Handle of the created motion data.
		*/
		template<typename TState1,typename TState2>
		MotionHandle bindWithState(TState1* state1,TState2* state2, std::function<void(TValue&, TState1*,TState2*)> action)
		{
			setMotionData();
			setCallbackData<TState1,TState2>(state1,state2, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object. Unlike the regular Bind method, it avoids allocation by closure by passing an object.
		*
		* @tparam TState1 : Type of state1
		* @tparam TState2 : Type of state2
		* @tparam TState3 : Type of state3
		* @param [in] state1 : Motion state1
		* @param [in] state2 : Motion state2
		* @param [in] state3 : Motion state3
		* @param[in] action : Action that handles binding
		*
		* @return Handle of the created motion data.
		*/
		template<typename TState1,typename TState2,typename TState3>
		MotionHandle bindWithState(TState1* state1,TState2* state2,TState3* state3, std::function<void(TValue&, TState1*,TState2*,TState3*)> action)
		{
			setMotionData();
			setCallbackData<TState1,TState2,TState3>(state1,state2,state3, action);
			return schedule();
		}
	private:
		void setMotionData()
		{
			m_motionData.Core.Status = MotionStatus::Scheduled;
		}
		void setCallbackData(std::function<void(TValue&)> action)
		{
			m_callbackData.setUpdateAction<TValue>(action);
		}
		template<typename TState>
		void setCallbackData(TState* state,std::function<void(TValue&,TState*)> action)
		{
			m_callbackData.setUpdateActionWithState1<TValue, TState>(state, action);
		}
		template<typename TState1,typename TState2>
		void setCallbackData(TState1* state1,TState2* state2, std::function<void(TValue&, TState1*,TState2*)> action)
		{
			m_callbackData.setUpdateActionWithState1<TValue, TState1,TState2>(state1,state2, action);
		}
		template<typename TState1,typename TState2,typename TState3>
		void setCallbackData(TState1* state1,TState2* state2,TState3* state3, std::function<void(TValue&, TState1*,TState2*,TState3*)> action)
		{
			m_callbackData.setUpdateActionWithState1<TValue, TState1,TState2,TState3>(state1,state2,state3, action);
		}
		MotionHandle schedule()
		{
			if (m_bindOnSchedule && m_callbackData.hasUpdateAction())
			{
				auto bindValue = evaluate<TValue>(
					m_motionData.StartValue,
					m_motionData.EndValue,
					MotionEvaluationContext{ EaseUtility::evaluate(0.0,m_motionData.Core.Ease) }
				);
				m_callbackData.invoke(bindValue);
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

