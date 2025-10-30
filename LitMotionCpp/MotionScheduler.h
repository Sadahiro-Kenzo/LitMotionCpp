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
		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static std::shared_ptr<IMotionScheduler<TValue,TOptions>> m_manualMotionScheduler;
		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static std::shared_ptr<IMotionScheduler<TValue,TOptions>> m_defaultMotionScheduler;
	public:
		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static std::weak_ptr<IMotionScheduler<TValue,TOptions>> getManual()
		{
			if (!m_manualMotionScheduler<TValue,TOptions>)
			{
				m_manualMotionScheduler<TValue,TOptions> = std::make_shared<ManualMotionScheduler<TValue,TOptions>>();
			}

			return m_manualMotionScheduler<TValue,TOptions>;
		}

		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static void setDefault(std::shared_ptr<IMotionScheduler<TValue,TOptions>> scheduler)
		{
			m_defaultMotionScheduler<TValue,TOptions> = scheduler;
		}
		template<typename TValue,typename TOptions>
			requires std::derived_from<TOptions, IMotionOptions>
		static std::weak_ptr<IMotionScheduler<TValue,TOptions>> getDefault() { return m_defaultMotionScheduler<TValue,TOptions>; }
	};

	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::shared_ptr<IMotionScheduler<TValue,TOptions>> MotionScheduler::m_manualMotionScheduler;

	template<typename TValue,typename TOptions>
		requires std::derived_from<TOptions, IMotionOptions>
	std::shared_ptr<IMotionScheduler<TValue,TOptions>> MotionScheduler::m_defaultMotionScheduler;

}//namespace

