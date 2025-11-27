#pragma once
#include <AnimationCurve.h>
class DirectXMathAnimationCurve :
    public LitMotionCpp::AnimationCurve
{
public:
    DirectXMathAnimationCurve() = delete;
    explicit DirectXMathAnimationCurve(const LitMotionCpp::Keyframe* begin, const LitMotionCpp::Keyframe* end, LitMotionCpp::WrapMode preWrapMode = LitMotionCpp::WrapMode::Default, LitMotionCpp::WrapMode postWrapMode = LitMotionCpp::WrapMode::Default)
        : LitMotionCpp::AnimationCurve(begin, end,preWrapMode,postWrapMode)
    {
    }
	virtual ~DirectXMathAnimationCurve() = default;

    static void registerFactory()
    {
        LitMotionCpp::AnimationCurve::setFactory([](const LitMotionCpp::Keyframe* begin, const LitMotionCpp::Keyframe* end, LitMotionCpp::WrapMode preWrapMode, LitMotionCpp::WrapMode postWrapMode) -> LitMotionCpp::AnimationCurve*
        {
            return new DirectXMathAnimationCurve(begin, end,preWrapMode,postWrapMode);
        });
	}
protected:
	float interpolate(float time, const LitMotionCpp::Keyframe& key0, const LitMotionCpp::Keyframe& key1) const override;
};

