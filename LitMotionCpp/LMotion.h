#pragma once
#include "MotionBuilder.h"
#include "MotionScheduler.h"
#include "ManualMotionDispatcher.h"
#include "PrimitiveMotionAdapter.h"

namespace LitMotionCpp
{
	class LMotion
	{
	public:
		/**
		* @brief Create a builder for building motion.
		* 
		* @param [in] from : Start value
		* @param [in] to : End value
		* @param [in] duration : Duration
		* 
		* @return Created motion builder
		*/
		static MotionBuilder<float> Create(float from, float to, float duration);
	};
}//namespace

