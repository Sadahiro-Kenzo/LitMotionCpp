#include "pch.h"
#include <format>
#include "ICanvas.h"
#include "TextLabelExtensions.h"

namespace LitMotionCpp::Sample
{
	const size_t BUFFER_SIZE = 256;
	char g_textBuffer[BUFFER_SIZE];

	void TextLabelExtensions::ToText(float value, ITextLabel* target)
	{
		auto [end, _] = std::format_to_n(g_textBuffer, BUFFER_SIZE - 1, "{}", value);
		*end = '\0';
		target->SetText(g_textBuffer);
	}
}//namespace
