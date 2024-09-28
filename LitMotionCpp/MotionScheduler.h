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
	};

	template<typename TValue>
	std::shared_ptr<IMotionScheduler<TValue>> MotionScheduler::m_manualMotionScheduler;

}//namespace

