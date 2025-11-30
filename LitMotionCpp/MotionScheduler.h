#pragma once
#include <memory>
#include "MotionHandle.h"
#include "MotionDispatcher.h"
#include "ManualMotionDispatcher.h"

namespace LitMotionCpp
{
	/**
	* @brief Schedulers available in Runtime.
	*/
	class MotionScheduler
	{
	public:
		enum class Type :uint8_t
		{
			Invalid,
			PlayerLoop,
			Manual,
		};

	private:
		int m_playerLoopTiming;
		MotionTimeKind m_timeKind;
		Type m_type;
	private:
		MotionScheduler(int playerLoopTiming, MotionTimeKind timeKind, Type type)
			:m_playerLoopTiming(playerLoopTiming)
			,m_timeKind(timeKind)
			,m_type(type)
		{
		}
	public:
		MotionScheduler()
			:m_playerLoopTiming(0)
			, m_timeKind(MotionTimeKind::Time)
			, m_type(Type::Invalid)
		{
		}
		MotionScheduler& operator=(const MotionScheduler&) = default;

		int getPlayerLoopTiming() const { return m_playerLoopTiming; }
		MotionTimeKind getTimeKind() const { return m_timeKind; }
		Type getType() const { return m_type; }

		template<typename TValue, typename TOptions, typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		void ensureStorageCapacity(size_t capacity)
		{
			if (m_type == Type::PlayerLoop)
			{
				return MotionDispatcher::ensureStorageCapacity<TValue, TOptions, TAdapter>(capacity,m_playerLoopTiming);
			}
			if (m_type == Type::Manual)
			{
				return ManualMotionDispatcher::ensureStorageCapacity<TValue, TOptions, TAdapter>(capacity);
			}
		}

		template<typename TValue, typename TOptions, typename TAdapter>
			requires std::derived_from<TOptions, IMotionOptions>&& std::derived_from<TAdapter, IMotionAdapter<TValue, TOptions>>
		MotionHandle schedule(MotionData<TValue, TOptions>& data,MotionCallbackData& callbackData)
		{
			data.Core.Parameters.TimeKind = m_timeKind;
			if (m_type == Type::PlayerLoop)
			{
				return MotionDispatcher::schedule<TValue, TOptions, TAdapter>(data, callbackData,m_playerLoopTiming);
			}
			if (m_type == Type::Manual)
			{
				return ManualMotionDispatcher::schedule<TValue, TOptions, TAdapter>(data, callbackData);
			}

			assert(false && "Invalid MotionScheduler Type");
			return MotionHandle();
		}


		static MotionScheduler createPlayerLoop(int playerLoopTiming, MotionTimeKind timeKind = MotionTimeKind::Time)
		{
			return MotionScheduler(playerLoopTiming, timeKind, Type::PlayerLoop);
		}

		static MotionScheduler& getDefaultScheduler();
		static void setDefaultScheduler(const MotionScheduler& scheduler);

		static MotionScheduler& getUpdate();

		static MotionScheduler& getManual();
	};
}//namespace

