#include "pch.h"
#include <chrono>
#include "BenchmarkScene.h"

#include "ICanvas.h"

using namespace LitMotionCpp;
using namespace DirectX;

namespace LitMotionCpp::Sample
{
	struct Box
	{
		int value;
	};
	std::array<Box, 10000> boxes;

	void doNothing(float, int)
	{
		// do nothing
	}

	BenchmarkScene::BenchmarkScene(std::unique_ptr<ICanvas>&& initialCanvas)
		:SampleScene(std::move(initialCanvas))
		, m_scheduler(MotionScheduler::createPlayerLoop(1))
	{
		auto& canvas = getCanvas();

		m_scheduler.ensureStorageCapacity<float, NoOptions, FloatMotionAdapter<float>>(boxes.size());

		TextLabelSpec spec{
			.text = "7. Benchmark",
			.position = {0.0f,240.0f},
			.hAlign = HorizontalAlign::Center,
			.vAlign = VerticalAlign::Middle,
			.pivot = {0.5f,0.5f}
		};
		canvas.PushBackTextLabel(spec);

		sprintf_s(m_buffer.data(), m_buffer.size(), "Update %d Times per frame.", static_cast<int>(boxes.size()));
		spec.text = m_buffer.data();
		spec.position.y -= 60.0f;
		canvas.PushBackTextLabel(spec);

		spec.text = "Elapsed Time: ";
		spec.position.y -= 60.0f;
		m_elapsedText = canvas.PushBackTextLabel(spec);

		spec.text = "Min: Max: Average";
		spec.position.y -= 60.0f;
		m_minMaxAveText = canvas.PushBackTextLabel(spec);
	}

	void BenchmarkScene::onStart()
	{
		m_minElapsedTime = std::numeric_limits<double>::max();
		m_maxElapsedTime = 0.0;
		m_totalElapsedTime = 0.0;
		m_frameCount = 0;

		for(size_t i=0; i < boxes.size(); ++i)
		{
			boxes[i].value = static_cast<int>(i);
			// Create a dummy motion and start it
			auto handle = LMotion::create(0.0f, 1.0f, 3.0f)
				.withScheduler(m_scheduler)
				.bind<Box>(&(boxes[i]), [](float x, Box* state) {
				doNothing(x, state->value);
					});
			AddHandle(handle);
		}

		SampleScene::onStart();
	}

	using time_point = std::chrono::high_resolution_clock::time_point;

	time_point now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	double duration(const time_point& start, const time_point& end)
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000.0);
	}

	void BenchmarkScene::onUpdate(IInput& input)
	{
		constexpr size_t warmupFrames = 10;
		++m_frameCount;

		if (IsPlaying() && m_frameCount>warmupFrames)
		{
			auto start = now();
			MotionDispatcher::update(m_scheduler);
			auto end = now();
			auto elapsed = duration(start, end);

			m_totalElapsedTime += elapsed;
			m_minElapsedTime = std::min(m_minElapsedTime, elapsed);
			m_maxElapsedTime = std::max(m_maxElapsedTime, elapsed);
			auto average = m_totalElapsedTime / static_cast<double>(m_frameCount - warmupFrames);

			sprintf_s(m_buffer.data(), m_buffer.size(), "Elapsed Time: %f ms", elapsed);
			if (auto text = m_elapsedText.lock())
			{
				text->SetText(m_buffer.data());
			}

			sprintf_s(m_buffer.data(), m_buffer.size(), "Min: %f ms  Max:%f ms  Average:%f ms", m_minElapsedTime,m_maxElapsedTime,average);
			if (auto text = m_minMaxAveText.lock())
			{
				text->SetText(m_buffer.data());
			}
		}
		else
		{
			MotionDispatcher::update(m_scheduler);
		}

		SampleScene::onUpdate(input);
	}

	void BenchmarkScene::onEnd()
	{
	}

}// namespace