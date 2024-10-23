#pragma once
#include <MotionHandle.h>

class Utility
{
public:
	/**
	* @brief Update ManualMotionDispatcher by 1/60sec,while handle.isActive() return true.
	* 
	* @param [in] handle : motion handle
	*/
	static void toYieldInteraction(LitMotionCpp::MotionHandle handle);

	/**
	* @brief Update ManualMotionDispatcher by 1/60sec,until spend waitTime.
	* 
	* @param [in] waitTime : wait time
	*/
	static void waitForSeconds(float waitTime);
};

