#include "pch.h"
#include <queue>
#include "Message.h"

namespace LitMotionCpp::Sample
{
	std::queue<std::tuple<Message::Type, int>> messageQueue;

	bool Message::IsEmpty()
	{
		return messageQueue.empty();
	}

	void Message::Post(Type type, int value)
	{
		messageQueue.emplace(type, value);
	}

	std::tuple<Message::Type, int> Message::Fetch()
	{
		auto result(messageQueue.front());
		messageQueue.pop();
		return result;
	}
}// namespace

