#pragma once
#include <stdint.h>
#include "IMotionOptions.h"

namespace LitMotionCpp
{
	/*
	* @brief Specifies the rounding format for values after the decimal point.
	*/
	enum class RoundingMode : uint8_t
	{
		ToEven,
		AwayFromZero,
		ToZero,
		ToPositiveInfinity,
		ToNegativeInfinity
	};

	/*
	* @brief Options for integer type motion.
	*/
	struct IntegerOptions : public IMotionOptions
	{
		RoundingMode roundingMode = RoundingMode::ToEven;
		IntegerOptions(RoundingMode mode = RoundingMode::ToEven)
			: roundingMode(mode)
		{
		}

		bool operator==(const IntegerOptions& other) const
		{
			return roundingMode == other.roundingMode;
		}
	};

};//namespace LitMotionCpp