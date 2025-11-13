#pragma once
#include <concepts>
#include <functional>

namespace LitMotionCpp
{
	template<typename T>
	concept IsValueType = std::integral<T> || std::floating_point<T>;

	template<typename T>
	concept IsReferenceType = !IsValueType<T>;

	/*
	* @brief A structure that holds motion callbacks.
	*/
	class MotionCallbackData
	{
	private:
		int m_stateCount = 0;
		void* m_pState1 = nullptr;
		void* m_pState2 = nullptr;
		void* m_pState3 = nullptr;

		// original callback function pointer
		void* m_rawUpdateFunctionPtr = nullptr;

	public:
		bool IsCallbackRunning = false;
		bool CancelOnError = false;
		bool SkipValuesDuringDelay = true;
		std::function<void()> OnCompleteAction;
		std::function<void()> OnCancelAction;
		bool hasUpdateAction() { return m_rawUpdateFunctionPtr !=nullptr; }
	public:
		template<typename TValue, typename TState1>
		void setUpdateAction(TState1* pState1, void(*action)(TValue, TState1*))
		{
			m_stateCount = 1;
			m_pState1 = pState1;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue, typename TState1>
		void setUpdateAction(TState1* pState1, void(*action)(const TValue&, TState1*))
		{
			m_stateCount = 1;
			m_pState1 = pState1;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue, typename TState1, typename TState2>
		void setUpdateAction(TState1* pState1, TState2* pState2, void(*action)(TValue, TState1*, TState2*))
		{
			m_stateCount = 2;
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue, typename TState1, typename TState2>
		void setUpdateAction(TState1* pState1, TState2* pState2, void(*action)(const TValue&, TState1*, TState2*))
		{
			m_stateCount = 2;
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue, typename TState1, typename TState2, typename TState3>
		void setUpdateAction(TState1* pState1, TState2* pState2, TState3* pState3, void(*action)(TValue, TState1*, TState2*, TState3*))
		{
			m_stateCount = 3;
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_pState3 = pState3;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue, typename TState1, typename TState2, typename TState3>
		void setUpdateAction(TState1* pState1, TState2* pState2, TState3* pState3, void(*action)(const TValue&, TState1*, TState2*, TState3*))
		{
			m_stateCount = 3;
			m_pState1 = pState1;
			m_pState2 = pState2;
			m_pState3 = pState3;
			m_rawUpdateFunctionPtr = reinterpret_cast<void*>(action);
		}

		template<typename TValue>
			requires IsValueType<TValue>
		void invoke(TValue value)
		{
			if(!hasUpdateAction())
			{
				return;
			}

			switch (m_stateCount)
			{
			case 1:reinterpret_cast<void(*)(TValue, void*)>(m_rawUpdateFunctionPtr)(value, m_pState1); return;
			case 2:reinterpret_cast<void(*)(TValue, void*,void*)>(m_rawUpdateFunctionPtr)(value, m_pState1,m_pState2); return;
			case 3:reinterpret_cast<void(*)(TValue, void*,void*,void*)>(m_rawUpdateFunctionPtr)(value, m_pState1,m_pState2,m_pState3); return;
			default:
				break;
			}
		}


		template<typename TValue>
			requires IsReferenceType<TValue>
		void invoke(TValue& value)
		{
			if (!hasUpdateAction())
			{
				return;
			}

			switch (m_stateCount)
			{
			case 1:reinterpret_cast<void(*)(const TValue&, void*)>(m_rawUpdateFunctionPtr)(value, m_pState1); return;
			case 2:reinterpret_cast<void(*)(const TValue&, void*, void*)>(m_rawUpdateFunctionPtr)(value, m_pState1, m_pState2); return;
			case 3:reinterpret_cast<void(*)(const TValue&, void*, void*, void*)>(m_rawUpdateFunctionPtr)(value, m_pState1, m_pState2, m_pState3); return;
			default:
				break;
			}
		}
	};

}//namespace