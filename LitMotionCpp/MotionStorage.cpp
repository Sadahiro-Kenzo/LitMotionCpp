#include "pch.h"
#include "MotionStorage.h"

namespace LitMotionCpp
{
	bool StorageEntry::operator==(const StorageEntry& other)
	{
		return other.Next == Next && other.DenseIndex == DenseIndex && other.Version == Version;
	}

	/*
	* StorageEntryList
	*/

	StorageEntryList::StorageEntryList(size_t capacity)
		:m_entries{capacity}
	{
		reset();
	}

	StorageEntry& StorageEntryList::operator[](size_t index)
	{
		return m_entries[index];
	}

	/*
	* @brief Reset free entry list
	* 
	* @param [in] startEntry : index of top free entry
	*/
	void StorageEntryList::reset(size_t startEntry)
	{
		assert(startEntry< m_entries.size());

		int nextEntry = static_cast<int>(startEntry) + 1;
		for (auto entry = m_entries.begin() + startEntry; entry != m_entries.end(); entry++)
		{
			entry->Next = nextEntry;
			entry->DenseIndex = -1;
			entry->Version = 1;

			nextEntry++;
		}
		m_freeEntry = static_cast<int>(startEntry);
		m_entries.back().Next = -1;
	}

	void StorageEntryList::ensureCapacity(size_t capacity)
	{
		assert(m_entries.size() < capacity);
		auto currentSize = m_entries.size();
		m_entries.resize(capacity);
		reset(currentSize);
	}

	StorageEntry StorageEntryList::alloc(int denseIndex, int* pEntryIndex)
	{
		if (m_freeEntry == -1)
		{
			ensureCapacity(m_entries.size() * 2);
		}

		// Find free entry
		*pEntryIndex = m_freeEntry;
		auto& entry = m_entries[*pEntryIndex];
		m_freeEntry = entry.Next;
		entry.Next = -1;
		entry.DenseIndex = denseIndex;

		return entry;
	}

	void StorageEntryList::free(int index)
	{
		auto& entry = m_entries[index];
		entry.Next = m_freeEntry;
		entry.Version++;
		m_freeEntry = index;
	}

	void StorageEntryList::reset()
	{
		reset(0);
	}
}//namespace