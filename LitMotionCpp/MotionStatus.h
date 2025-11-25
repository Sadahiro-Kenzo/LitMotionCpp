#pragma once
namespace LitMotionCpp
{
	enum class MotionStatus
	{
		None,
		Scheduled,
		Delayed,
		Playing,
		Completed,
		Canceled,
		Disposed,
	};

	enum class Ease
	{
		Linear,
		InSine,
		OutSine,
		InOutSine,
		InQuad,
		OutQuad,
		InOutQuad,
		InCublic,
		OutCublic,
		InOUtCublic,
		InQuart,
		OutQuart,
		InOutQuart,
		InQuint,
		OutQuint,
		InOutQuint,
		InExpo,
		OutExpo,
		InOutExpo,
		InCirc,
		OutCirc,
		InOutCirc,
		InElastic,
		OutElastic,
		InOutElastic,
		InBack,
		OutBack,
		InOutBack,
		InBounce,
		OutBounce,
		InOutBounce,
		CustomAnimationCurve,
	};

	/*
	* @brief Specifies the behavior of WithDelay.
	*/
	enum class DelayType
	{
		FirstLoop,//!< Delay when starting playback
		EveryLoop,//!< Delay every loop
	};

	/**
	* @brief Specifies the behavior when repeating the animation with `WithLoops`.
	*/
	enum class LoopType
	{
		Restart,	//!< Repeat from beginning.
		Flip,		//!< Cycles back and forth between the end and start values.
		Incremental,//!< Increase the value each time the repeats.
		Yoyo,		//!< Cycles back and forth between the end and start values.
	};
}//namespace
