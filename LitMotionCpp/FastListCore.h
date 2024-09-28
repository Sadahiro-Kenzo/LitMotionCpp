#pragma once
#include <span>
#include <vector>

namespace LitMotionCpp
{
	template<typename T>
	class FastListCore
	{
	private:
		std::vector<T> m_array;
	public:
		void add(T element)
		{
			m_array.push_back(element);
		}

		void removeAtSwapback(size_t index)
		{
			m_array[index] = m_array.back();
			m_array.pop_back();
		}

		void ensureCapacity(size_t capacity)
		{
			m_array.reserve(capacity);
		}

		constexpr T& operator[](size_t index)
		{
			return m_array[index];
		}

		constexpr size_t size() const
		{
			return m_array.size();
		}

		std::span<T> asSpan()
		{
			return std::span<T>(m_array);
		}
	};
}//namespace