#include "pch.h"
#include <LMotion.h>
#include "Utility.h"

using namespace LitMotionCpp;

constexpr float delta = 1.0f / 60.0f;

void Utility::toYieldInteraction(MotionHandle handle)
{
	ManualMotionDispatcher::update(delta);
	while (handle.isActive())
	{
		ManualMotionDispatcher::update(delta);
	}
}

void Utility::waitForSeconds(float waitTime)
{
	auto time = ManualMotionDispatcher::getTime();
	ManualMotionDispatcher::update(delta);
	while (ManualMotionDispatcher::getTime()-time<waitTime)
	{
		ManualMotionDispatcher::update(delta);
	}
}