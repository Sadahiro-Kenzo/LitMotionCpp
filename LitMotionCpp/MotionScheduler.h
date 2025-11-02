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
		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static std::shared_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> m_manualMotionScheduler;
		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static std::shared_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> m_defaultMotionScheduler;
	public:
		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static std::weak_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> getManual()
		{
			if (!m_manualMotionScheduler<TValue,TOptions, TAdapter>)
			{
				m_manualMotionScheduler<TValue,TOptions, TAdapter> = std::make_shared<ManualMotionScheduler<TValue,TOptions, TAdapter>>();
			}

			return m_manualMotionScheduler<TValue,TOptions, TAdapter>;
		}

		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static void setDefault(std::shared_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> scheduler)
		{
			m_defaultMotionScheduler<TValue,TOptions, TAdapter> = scheduler;
		}
		template<typename TValue,typename TOptions,typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		static std::weak_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> getDefault() { return m_defaultMotionScheduler<TValue,TOptions, TAdapter>; }
	};

	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::shared_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> MotionScheduler::m_manualMotionScheduler;

	template<typename TValue,typename TOptions,typename TAdapter>
		requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
	std::shared_ptr<IMotionScheduler<TValue,TOptions, TAdapter>> MotionScheduler::m_defaultMotionScheduler;

}//namespace

