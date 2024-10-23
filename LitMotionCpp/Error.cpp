#include "pch.h"
#include <cassert>
#include <stdexcept>
#include "Error.h"

namespace LitMotionCpp
{
	void Error::playbackSpeedMustBeZeroOrGreater()
	{
		throw std::out_of_range("Playback speed must be 0 or greater.");
	}
}//namespace

