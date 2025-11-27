#include "pch.h"
#include "DirectXMathAnimationCurve.h"

using namespace DirectX;
using namespace LitMotionCpp;

const XMMATRIX CurveMatrix = XMMATRIX
(
	-1.0f, 3.0f, -3.0f, 1.0f,
	3.0f, -6.0f, 3.0f, 0.0f,
	-3.0f, 3.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f
);

float DirectXMathAnimationCurve::interpolate(float time, const Keyframe& keyframe, const Keyframe& nextKeyframe) const
{
	const float s = nextKeyframe.time - keyframe.time;
	const float outWeight = (keyframe.weightedMode == WeightedMode::Out || keyframe.weightedMode == WeightedMode::Both) ? keyframe.outWeight : 1.0f / 3.0f;
	const float inWeight = (nextKeyframe.weightedMode == WeightedMode::In || nextKeyframe.weightedMode == WeightedMode::Both) ? nextKeyframe.inWeight : 1.0f / 3.0f;
	XMVECTOR yCoord = XMVectorSet(keyframe.value, keyframe.value + outWeight * keyframe.outTangent * s, nextKeyframe.value - inWeight * nextKeyframe.inTangent * s, nextKeyframe.value);
	auto curveYCoords = XMVector4Transform(yCoord, CurveMatrix);

	const float t = (time - keyframe.time) / s;
	const float t2 = t * t;
	XMVECTOR powerSeries = XMVectorSet(t2 * t, t2, t, 1.0f);

	return XMVectorGetX(XMVector4Dot(powerSeries,curveYCoords));
}

