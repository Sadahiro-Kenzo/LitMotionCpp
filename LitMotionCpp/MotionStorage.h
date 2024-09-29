#pragma once
#include <cassert>
#include <memory>
#include <span>
#include <tuple>
#include <vector>
#include "MotionData.h"
#include "MotionCallbackData.h"
#include "MotionAdapter.h"
#include "MotionHandle.h"
#include "MotionStatus.h"
#include "EaseUtility.h"

namespace LitMotionCpp
{
	struct StorageEntry
	{
		int Next = -1;
		int DenseIndex=-1;
		int Version=1;

		bool operator==(const StorageEntry&);
	};

	class IMotionStorage
	{
	public:
		virtual bool isActive(const MotionHandle&) = 0;
		virtual void cancel(const MotionHandle&) = 0;
		virtual void complete(const MotionHandle&) = 0;
		virtual MotionDataCore& getDataRef(const MotionHandle&) = 0;
		virtual MotionCallbackDataCore& getCallbackDataRef(const MotionHandle&) = 0;
	};

	class StorageEntryList final
	{
	private:
		std::vector<StorageEntry> m_entries;
		int m_freeEntry = 0;
	public:
		StorageEntryList(size_t capacity=32);
		StorageEntry& operator[](size_t index);
		StorageEntry alloc(int denseIndex, int* pEntryIndex);
		void free(int index);
		void reset();
	private:
		void ensureCapacity(size_t capacity);
		void reset(size_t startEntry);
	};

	template<typename TValue>
	class MotionStorage:public IMotionStorage
	{
	private:
		const int m_storageId;
		static const size_t InitialCapacity = 8;
		StorageEntryList m_entries;
		std::vector<int> m_toEntryIndex;
		std::vector<MotionData<TValue>> m_dataArray;
		std::vector<MotionCallbackData<TValue>> m_callbacksArray;
		int m_tail;
	public:
		MotionStorage(int id)
			:m_storageId{ id }
			,m_entries{InitialCapacity}
			,m_toEntryIndex{InitialCapacity}
			,m_dataArray{InitialCapacity}
			,m_callbacksArray{InitialCapacity}
			, m_tail{ 0 }
		{
			m_toEntryIndex.resize(InitialCapacity);
		}

		int getStorageId() { return m_storageId; }

		std::span<MotionData<TValue>> getDataSpan()
		{
			return std::span<MotionData<TValue>>(m_dataArray.begin(), m_tail);
		};
		std::span<MotionCallbackData<TValue>> getCallbacksSpan()
		{
			return std::span<MotionCallbackData<TValue>>(m_callbacksArray.begin(), m_tail);
		};

		std::tuple<int, int> append(const MotionData<TValue>& data, const MotionCallbackData<TValue>& callbacks)
		{
			if (m_tail == m_dataArray.size())
			{
				auto newSize = m_tail * 2;
				m_toEntryIndex.resize(newSize);
				m_dataArray.resize(newSize);
				m_callbacksArray.resize(newSize);
			}

			int entryIndex;
			auto entry = m_entries.alloc(m_tail, &entryIndex);
			m_toEntryIndex[m_tail] = entryIndex;
			m_dataArray[m_tail] = data;
			m_callbacksArray[m_tail] = callbacks;

			m_tail++;
			return { entryIndex,entry.Version };
		}

		void removeAt(int denseIndex)
		{
			m_tail--;

			// swap elements
			m_dataArray[denseIndex] = m_dataArray[m_tail];
			m_callbacksArray[denseIndex] = m_callbacksArray[m_tail];

			// swap entry indexes
			auto prevEntryIndex = m_toEntryIndex[denseIndex];
			auto currentEntryIndex = m_toEntryIndex[denseIndex] = m_toEntryIndex[m_tail];

			// update entry
			assert(currentEntryIndex != -1);
			m_entries[currentEntryIndex].DenseIndex = denseIndex;

			// free entry
			assert(prevEntryIndex != -1);
			m_entries.free(prevEntryIndex);
		}

		void removeAll(std::span<int> indexes)
		{
			auto entryIndexes = std::make_unique<int[]>(indexes.size());
			auto pEntryIndex = entryIndexes.get();
			for (auto i = indexes.begin(); i != indexes.end(); i++)
			{
				(*pEntryIndex) = m_toEntryIndex[*i];
				pEntryIndex++;
			}
			
			pEntryIndex = entryIndexes.get();
			auto pEntryIndexEnd = pEntryIndex + indexes.size();
			for (; pEntryIndex != pEntryIndexEnd; pEntryIndex++)
			{
				removeAt(m_entries[*pEntryIndex].DenseIndex);
			}
		}

		virtual bool isActive(const MotionHandle& handle) override
		{
			auto& entry = m_entries[handle.Index];
			auto denseIndex = entry.DenseIndex;
			if (denseIndex < 0 || denseIndex >= m_dataArray.size())
			{
				return false;
			}

			auto version = entry.Version;
			if (version < 0 || version != handle.Version)
			{
				return false;
			}

			auto status = m_dataArray[denseIndex].Core.Status;
			return status == MotionStatus::Scheduled || status == MotionStatus::Delayed || status == MotionStatus::Playing;
		}

		virtual void cancel(const MotionHandle& handle) override
		{
			auto& entry = m_entries[handle.Index];
			auto denseIndex = entry.DenseIndex;
			assert(0 <= denseIndex && denseIndex < m_tail);

			auto& motion = getDataSpan()[denseIndex];
			auto version = entry.Version;
			assert(0 < version && version == handle.Version && motion.Core.Status != MotionStatus::None);

			motion.Core.Status = MotionStatus::Canceled;
			auto& callbackData = getCallbacksSpan()[denseIndex];
			if (callbackData.OnCancelAction)
			{
				callbackData.OnCancelAction();
			}
		}

		virtual void complete(const MotionHandle& handle) override
		{
			auto& entry = m_entries[handle.Index];
			auto denseIndex = entry.DenseIndex;
			assert(0 <= denseIndex && denseIndex < m_tail);

			auto& motion = getDataSpan()[denseIndex];
			auto version = entry.Version;
			assert(0 < version && version == handle.Version && motion.Core.Status != MotionStatus::None);

			if (motion.Core.Loops < 0)
			{
				//	ToDo: Debug Log Message
				return;
			}

			auto& callbackData = getCallbacksSpan()[denseIndex];
			assert(!callbackData.IsCallbackRunning);
			callbackData.IsCallbackRunning = true;

			// To avoid duplication of Complete processing, it is treated as canceled internally.
			motion.Core.Status = MotionStatus::Canceled;

			auto endProgress = 1.0f;
			switch (motion.Core.LoopType)
			{
			case LoopType::Yoyo:endProgress = motion.Core.Loops % 2 == 0 ? 0.0f : 1.0f; break;
			case LoopType::Incremental: endProgress = static_cast<float>(motion.Core.Loops); break;
			}

			auto easedEndProgress = EaseUtility::evaluate(endProgress, motion.Core.Ease);	//	ToDo: AnimationCurve.evaluate

			MotionEvaluationContext context{ easedEndProgress };
			auto endValue = evaluate(motion.StartValue, motion.EndValue, context);

			callbackData.UpdateAction(endValue);
			if (callbackData.OnCompleteAction)
			{
				callbackData.OnCompleteAction();
			}

			callbackData.IsCallbackRunning = false;
		}

		virtual MotionDataCore& getDataRef(const MotionHandle& handle) override
		{
			return m_dataArray[m_entries[handle.Index].DenseIndex].Core;
		}

		virtual MotionCallbackDataCore& getCallbackDataRef(const MotionHandle& handle) override
		{
			return m_callbacksArray[m_entries[handle.Index].DenseIndex];
		}

		void reset()
		{
			m_entries.reset();

			// ToDo : must be zero clear or initialize?
			//m_toEntryIndex.clear();
			//m_dataArray.clear();
			//m_callbacksArray.clear();

			m_tail = 0;
		}
	};

}//namespace

