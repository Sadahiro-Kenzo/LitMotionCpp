#pragma once
#include <cassert>
#include <functional>
#include <memory>
#include "MotionData.h"
#include "MotionScheduler.h"
#include "MotionStatus.h"
#include "MotionCallbackData.h"
#include "PrimitiveMotionAdapter.h"
#include "EaseUtility.h"
#include "AnimationCurve.h"

namespace LitMotionCpp
{
	/**
	* @brief Supports construction, scheduling, and binding of motion entities.
	* 
	* @tparam TValue : The type of value to animate
	*/
	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions> && std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	class MotionBuilder
	{
	private:
		MotionScheduler m_scheduler;
		std::function<void(TValue)> m_updateAction;
		MotionData<TValue,TOptions> m_motionData;
		MotionCallbackData m_callbackData;
		bool m_bindOnSchedule = false;
		const Keyframe* m_customCurveBegin = nullptr;
		const Keyframe* m_customCurveEnd = nullptr;
	public:
		MotionBuilder() = delete;
		MotionBuilder(const TValue& from, const TValue& to, float duration)
			:m_motionData{from,to,duration}
		{
		}

		~MotionBuilder()
		{
			if(m_motionData.Core.Parameters.AnimationCurve)
			{
				delete m_motionData.Core.Parameters.AnimationCurve;
				m_motionData.Core.Parameters.AnimationCurve = nullptr;
			}
		}

		/**
		* @brief Specify easing for motion.
		*
		* @param [in] ease : The type of easing
		*
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue,TOptions,TAdapter>& withEase(Ease ease)
		{
			m_motionData.Core.Parameters.Ease = ease;
			return *this;
		}

		/**
		* @brief Specify easing for motion.
		*
		* keyframe array is not copied, the created instance has reference to the keyframe array [begin,end).
		* keyframes must be sorted by time in ascending order.
		*
		* @param [in] begin : Pointer to the beginning of the keyframe array
		* @param [in] end : Pointer to the end of the keyframe array
		*
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withEase(const Keyframe* begin,const Keyframe* end)
		{
			m_motionData.Core.Parameters.Ease = Ease::CustomAnimationCurve;
			m_customCurveBegin = begin;
			m_customCurveEnd = end;
			return *this;
		}

		/**
		* @brief Specify the delay time when the motion starts.
		*
		* @param [in] delay : Delay time (seconds)
		* @param [in] delayType : Delay type
		* @param [in] skipValuesDuringDelay : Whether to skip updating values during the delay time.
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withDelay(float delay,DelayType delayType=DelayType::FirstLoop,bool skipValuesDuringDelay=true)
		{
			m_motionData.Core.Parameters.Delay = delay;
			m_motionData.Core.Parameters.DelayType = delayType;
			m_callbackData.SkipValuesDuringDelay = skipValuesDuringDelay;
			return *this;
		}

		/**
		* @brief Specify the number of times the motion is repeated. If specified as less than 0, the motion will continue to play until manually completed or canceled.
		* 
		* @param [in] loops : Number of loops
		* @param [in] loopTpe : Behavior at the end of each loop
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withLoops(int loops,LoopType loopType=LoopType::Restart)
		{
			m_motionData.Core.Parameters.Loops = loops;
			m_motionData.Core.Parameters.LoopType = loopType;
			return *this;
		}

		/**
		* @brief Specify special parameters for each motion data.
		*
		* @param [in] options : Option value to specify
		*
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withOptions(TOptions options)
		{
			m_motionData.Options = options;
			return *this;
		}

		/**
		* @brief Specify the callback when canceled.
		* 
		* @param [in] callback : Callback when canceled.
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withOnCancel(std::function<void()> callback)
		{
			m_callbackData.OnCancelAction = callback;
			return *this;
		}

		/**
		* @brief Specify the callback when playback ends.
		*
		* @param [in] callback : Callback when playback ends.
		*
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withOnComplete(std::function<void()> callback)
		{
			m_callbackData.OnCompleteAction = callback;
			return *this;
		}

		/**
		* @brief Cancel Motion when an exception occurs during Bind processing.
		*
		* @param [in] cancelOnError : Whether to cancel on error.
		*
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withCancelOnError(bool cancelOnError=true)
		{
			m_callbackData.CancelOnError = cancelOnError;
			return *this;
		}

		/**
		* @brief Bind values when scheduling the motion.
		* 
		* @param [in] bindOnSchedule : Whether to bind on sheduling
		* 
		* @return This builder to allow chaining multiple method calls.
		*/
		MotionBuilder<TValue, TOptions, TAdapter>& withBindOnSchedule(bool bindOnSchedule = true)
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
		MotionBuilder<TValue, TOptions, TAdapter>& withScheduler(const MotionScheduler& scheduler)
		{
			m_scheduler = scheduler;
			return *this;
		};

		/**
		* @brief Create motion and play it without binding it to a specific object.
		* 
		* @return Handle of the created motion data.
		*/
		MotionHandle runWithoutBinding()
		{
			setMotionData();
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
		* 
		* @tparam TState : Type of state
		* @param [in] state : Motion state
		* @param[in] action : Action that handles binding
		* 
		* @return Handle of the created motion data.
		*/
		template<typename TState>
		MotionHandle bind(TState* state,void(*action)(TValue, TState*)) requires IsValueType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue, TState>(state, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
		*
		* @tparam TState : Type of state
		* @param [in] state : Motion state
		* @param[in] action : Action that handles binding
		*
		* @return Handle of the created motion data.
		*/
		template<typename TState>
		MotionHandle bind(TState* state, void(*action)(const TValue&, TState*)) requires IsReferenceType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue, TState>(state, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
		*
		* @tparam TState1 : Type of state1
		* @tparam TState2 : Type of state2
		* @param [in] state1 : Motion state1
		* @param [in] state2 : Motion state2
		* @param[in] action : Action that handles binding
		*
		* @return Handle of the created motion data.
		*/
		template<typename TState1, typename TState2>
		MotionHandle bind(TState1* state1, TState2* state2, void(*action)(TValue, TState1*, TState2*)) requires IsValueType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue, TState1, TState2>(state1, state2, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
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
		MotionHandle bind(TState1* state1,TState2* state2, void(*action)(const TValue&, TState1*, TState2*)) requires IsReferenceType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue,TState1,TState2>(state1,state2, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
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
		template<typename TState1, typename TState2, typename TState3>
		MotionHandle bind(TState1* state1, TState2* state2, TState3* state3, void(*action)(TValue, TState1*, TState2*, TState3*)) requires IsValueType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue, TState1, TState2, TState3>(state1, state2, state3, action);
			return schedule();
		}

		/**
		* @brief Create motion and bind it to a specific object.
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
		MotionHandle bind(TState1* state1,TState2* state2,TState3* state3, void(*action)(const TValue&, TState1*, TState2*, TState3*)) requires IsReferenceType<TValue>
		{
			setMotionData();
			m_callbackData.setUpdateAction<TValue,TState1,TState2,TState3>(state1,state2,state3, action);
			return schedule();
		}
	private:
		void setMotionData()
		{
			m_motionData.Core.State.Status = MotionStatus::Scheduled;

			if (m_customCurveBegin && m_customCurveEnd)
			{
				m_motionData.Core.Parameters.AnimationCurve = AnimationCurve::create(m_customCurveBegin, m_customCurveEnd);
				m_customCurveBegin = nullptr;
				m_customCurveEnd = nullptr;
			}
		}
		MotionHandle schedule()
		{
			if (m_bindOnSchedule && m_callbackData.hasUpdateAction())
			{
				TAdapter adapter;
				auto bindValue = adapter.evaluate(
					m_motionData.StartValue,
					m_motionData.EndValue,
					m_motionData.Options,
					MotionEvaluationContext{
						.Progress=m_motionData.Core.Parameters.Ease == Ease::CustomAnimationCurve ? m_motionData.Core.Parameters.AnimationCurve->evaluate(0.0f)
						:EaseUtility::evaluate(0.0f,m_motionData.Core.Parameters.Ease),
						.Time=0.0
					}
				);
				m_callbackData.invoke(bindValue);
			}

			MotionHandle handle;

			if (m_scheduler.getType()==MotionScheduler::Type::Invalid)
			{
				handle = MotionScheduler::getDefaultScheduler().schedule<TValue, TOptions, TAdapter>(m_motionData, m_callbackData);
			}
			else
			{
				handle=m_scheduler.schedule<TValue,TOptions,TAdapter>(m_motionData, m_callbackData);
			}
			m_motionData.Core.Parameters.AnimationCurve = nullptr; // Prevent double deletion

			return handle;
		}
	};
}//namespace

