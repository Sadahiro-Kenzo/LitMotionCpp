#include "pch.h"
#include "DirectXMotionAdapter.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace LitMotionCpp
{
	template<>
	DirectX::XMFLOAT4 evaluate<DirectX::XMFLOAT4>(const DirectX::XMFLOAT4& startValue, const DirectX::XMFLOAT4& endValue, const MotionEvaluationContext& context)
	{
		return Vector4::Lerp(startValue, endValue, context.Progress);
	}
}//namespace