#pragma once
#include <array>
#include "SampleScene.h"

namespace LitMotionCpp::Sample
{
    class BenchmarkScene : public SampleScene
    {
	private:
		MotionScheduler m_scheduler;
		std::array<char, 64> m_buffer;
		std::weak_ptr<ITextLabel> m_elapsedText;
		std::weak_ptr<ITextLabel> m_minMaxAveText;
		int m_frameCount = 0;
		double m_totalElapsedTime = 0.0;
		double m_minElapsedTime = std::numeric_limits<double>::max();
		double m_maxElapsedTime = 0.0;
	public:
		BenchmarkScene(std::unique_ptr<ICanvas>&& canvas);

		void onStart() override;
		void onUpdate(IInput&) override;
		void onEnd() override;
	};

}//namespace

