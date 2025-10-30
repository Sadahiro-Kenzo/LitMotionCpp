#pragma once
namespace LitMotionCpp::Sample
{
	class ITextLabel;

	/**
	* @brief Extension functions bind TextLabel property.
	*/
	struct TextLabelExtensions
	{
		static void ToText(float, ITextLabel*);
	};
}//namespace

