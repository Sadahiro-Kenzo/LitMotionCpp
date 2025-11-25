#pragma once
#include <vector>
#include <functional>
namespace LitMotionCpp
{
	enum class WeightedMode
	{
		None = 0,
		In = 1,
		Out = 2,
		Both = 3
	};

	struct Keyframe
	{
		float inTangent = 0.0f;
		float inWeight = 0.0f;
		float outTangent = 0.0f;
		float outWeight = 0.0f;
		float time = 0.0f;
		float value = 0.0f;
		WeightedMode weightedMode = WeightedMode::None;

		Keyframe() = default;
		Keyframe(float time, float value)
			: time(time), value(value)
		{
		}
		Keyframe(float time, float value, float inTangent, float outTangent)
			: time(time), value(value), inTangent(inTangent), outTangent(outTangent)
		{
		}
		Keyframe(float time, float value, float inTangent, float outTangent, float inWeight, float outWeight)
			: time(time), value(value), inTangent(inTangent), outTangent(outTangent), inWeight(inWeight), outWeight(outWeight), weightedMode(WeightedMode::Both)
		{
		}
	};

	enum class WrapMode
	{
		Once = 0,
		Loop = 1,
		PingPong = 2,
		Default = 3,
		ClampForever = 4
	};

	/*
	* @brief interface for animation curve
	*/
	class IAnimationCurve
	{
	public:
		virtual ~IAnimationCurve() = default;
		/*
		* @brief evaluate the curve at the given time
		*/
		virtual float evaluate(float time) const = 0;
		/*
		* @brief ping pong function
		* 
		* @param time input time
		* @param length half length of one ping pong cycle
		*/
		static float pingPong(const float time, const float length);
		/*
		* @brief wrap time according to wrap modes
		*/
		static float wrapTime(const float startTime,const float endTime,WrapMode preWrapMode,WrapMode postWrapMode,const float time);
	};

	/*
	* @brief implementation of animation curve
	*/
	class AnimationCurve :public IAnimationCurve
    {
    public:
        AnimationCurve() = delete;
		/*
		* @brief constructor
		* 
		* @see create method
		*/
        explicit AnimationCurve(const Keyframe* begin,const Keyframe* end,WrapMode preWrapMode=WrapMode::Default,WrapMode postWrapMode=WrapMode::Default)
            : m_begin(begin)
			, m_end(end)
			, m_preWrapMode(preWrapMode)
			, m_postWrapMode(postWrapMode)
        {
        }
        virtual ~AnimationCurve()=default;
		virtual float evaluate(float time) const override;

		/*
		* @brief factory method for creating AnimationCurve instances
		* 
		* uses the factory function set by setFactory method
		* keyframe array is not copied, the created instance has reference to the keyframe array [begin,end).
		* keyframes must be sorted by time in ascending order.
		* 
		* @param begin pointer to the beginning of keyframe array
		* @param end pointer to the end of keyframe array
		* @param preWrapMode wrap mode before the first keyframe
		* @param postWrapMode wrap mode after the last keyframe
		* 
		* @return pointer to the created AnimationCurve instance.
		*/
		static AnimationCurve* create(const Keyframe* begin, const Keyframe* end, WrapMode preWrapMode = WrapMode::Default, WrapMode postWrapMode = WrapMode::Default);
		static void setFactory(std::function<AnimationCurve* (const Keyframe*, const Keyframe*, WrapMode, WrapMode)> factory);
	protected:
		/*
		* @brief interpolation between two keyframes
		*/
		virtual float interpolate(float time, const Keyframe& keyframe, const Keyframe& nextKeyframe) const;
    private:
		const Keyframe* m_begin;
		const Keyframe* m_end;
		WrapMode m_preWrapMode;
		WrapMode m_postWrapMode;
    };

}// namespace

