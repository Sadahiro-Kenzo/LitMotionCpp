#pragma once
#include <memory>
#include "MotionHandle.h"
#include "ManualMotionScheduler.h"

namespace LitMotionCpp
{
	/**
	* @brief Schedulers available in Runtime.
	*/
	class MotionScheduler
	{
	private:
		template<typename TValue>
		static std::shared_ptr<IMotionScheduler<TValue>> m_manualMotionScheduler;
		template<typename TValue>
		static std::shared_ptr<IMotionScheduler<TValue>> m_defaultMotionScheduler;
	public:
		template<typename TValue>
		static std::weak_ptr<IMotionScheduler<TValue>> getManual()
		{
			if (!m_manualMotionScheduler<TValue>)
			{
				m_manualMotionScheduler<TValue> = std::make_shared<ManualMotionScheduler<TValue>>();
			}

			return m_manualMotionScheduler<TValue>;
		}

		template<typename TValue>
		static void setDefault(std::shared_ptr<IMotionScheduler<TValue>> scheduler)
		{
			m_defaultMotionScheduler<TValue> = scheduler;
		}
		template<typename TValue>
		static std::weak_ptr<IMotionScheduler<TValue>> getDefault() { return m_defaultMotionScheduler<TValue>; }
	};

	template<typename TValue>
	std::shared_ptr<IMotionScheduler<TValue>> MotionScheduler::m_manualMotionScheduler;

	template<typename TValue>
	std::shared_ptr<IMotionScheduler<TValue>> MotionScheduler::m_defaultMotionScheduler;

}//namespace

