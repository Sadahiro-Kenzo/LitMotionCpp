#pragma once
#include "IMotionOptions.h"

namespace LitMotionCpp
{
	/*
	* @brief A type indicating that motion has no special options. Specify in the type argument of MotionAdapter when the option is not required.
	*/
	struct NoOptions : public IMotionOptions
	{
	};

};//namespace LitMotionCpp