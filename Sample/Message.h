#pragma once
#include <tuple>

namespace LitMotionCpp::Sample
{
	class Message
	{
	public:
		enum class Type
		{
			Select,
			Close,
		};
		static bool IsEmpty();
		static void Post(Type,int);
		static std::tuple<Type,int> Fetch();
	};
}// namespace

