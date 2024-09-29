#pragma once
#include <LMotion.h>
#include "VertexTypes.h"
#include "DirectXMath.h"

namespace LitMotionCpp
{
	template<> DirectX::XMFLOAT4 evaluate(const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const MotionEvaluationContext&);
}//namespace
