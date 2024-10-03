#pragma once
#include <functional>

namespace LitMotionCpp
{
	/*
	* @brief A structure that holds motion callbacks.
	*/
	class MotionCallbackData
	{
	private:
		void* m_pState1;
		void* m_pState2;
		void* m_pState3;
		std::function<void(void)> m_updateAction;
		std::function<void(MotionCallbackData*, void*)> m_castToSettedTypeAction;	//!< Cast function for callback.
	public:
		bool IsCallbackRunning = false;
		bool CancelOnError = false;
		bool SkipValuesDuringDelay = true;
		std::function<void()> OnCompleteAction;
		std::function<void()> OnCancelAction;
		bool hasUpdateAction() { return m_updateAction.operator bool(); }

		template<typename TValue>
		void setUpdateAction(std::function<void(TValue&)> action)
		{
			m_updateAction = *(reinterpret_cast<decltype(m_updateAction)*>(&action));
			m_castToSettedTypeAction = [](MotionCallbackData* pCallbackData,void* value)
				{
					auto pUpdateAction = reinterpret_cast<std::function<void(TValue&)>*>(&(pCallbackData->m_updateAction));
					(*pUpdateAction)(*(reinterpret_cast<TValue*>(value)));
				};
		}

		template<typename TValue,typename TState1>
		void setUpdateActionWithState1(TState1* pState1,std::function<void(TValue&,TState1*)> action)
		{
			m_pState1 = pState1;
			m_updateAction = *(reinterpret_cast<decltype(m_updateAction)*>(&action));
			m_castToSettedTypeAction = [](MotionCallbackData* pCallbackData,void* value)
				{
					auto pUpdateAction = reinterpret_cast<std::function<void(TValue&,TState1*)>*>(&(pCallbackData->m_updateAction));
					(*pUpdateAction)(*(reinterpret_cast<TValue*>(value)),reinterpret_cast<TState1*>(pCallbackData->m_pState1));
				};
		}

		template<typename TValue, typename TState1,typename TState2>
		void setUpdateActionWithState1(TState1* pState1,TState2* pState2, std::function<void(TValue&, TState1*, TState2*)> action)
		{
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_updateAction = *(reinterpret_cast<decltype(m_updateAction)*>(&action));
			m_castToSettedTypeAction = [](MotionCallbackData* pCallbackData, void* value)
				{
					auto pUpdateAction = reinterpret_cast<std::function<void(TValue&, TState1*, TState2*)>*>(&(pCallbackData->m_updateAction));
					(*pUpdateAction)(
						*(reinterpret_cast<TValue*>(value)),
						reinterpret_cast<TState1*>(pCallbackData->m_pState1),
						reinterpret_cast<TState2*>(pCallbackData->m_pState2));
				};
		}

		template<typename TValue, typename TState1, typename TState2,typename TState3>
		void setUpdateActionWithState1(TState1* pState1, TState2* pState2, TState3* pState3,std::function<void(TValue&, TState1*, TState2*,TState3*)> action)
		{
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_pState3 = pState3;
			m_updateAction = *(reinterpret_cast<decltype(m_updateAction)*>(&action));
			m_castToSettedTypeAction = [](MotionCallbackData* pCallbackData, void* value)
				{
					auto pUpdateAction = reinterpret_cast<std::function<void(TValue&, TState1*, TState2*,TState3*)>*>(&(pCallbackData->m_updateAction));
					(*pUpdateAction)(
						*(reinterpret_cast<TValue*>(value)),
						reinterpret_cast<TState1*>(pCallbackData->m_pState1),
						reinterpret_cast<TState2*>(pCallbackData->m_pState2),
						reinterpret_cast<TState3*>(pCallbackData->m_pState3));
				};
		}

		template<typename TValue>
		void invoke(TValue& value)
		{
			if (m_castToSettedTypeAction)
			{
				m_castToSettedTypeAction(this, &value);
			}
		}
	};

}//namespace