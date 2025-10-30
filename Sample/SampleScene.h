#pragma once
#include <vector>
#include "LMotion.h"
#include "Interfaces.h"

namespace LitMotionCpp::Sample
{
    class SampleScene : public Scene
    {
    private:
        enum class State
        {
            Ready,
            Playing,
			Completed,
		};
		State m_state = State::Ready;
		std::vector<MotionHandle> m_handles;
    protected:
		void Complete();
        void AddHandle(const MotionHandle& handle) { m_handles.push_back(handle); }
        bool IsPlaying() const { return m_state == State::Playing; }
    public:
        SampleScene() = delete;
        SampleScene(std::unique_ptr<ICanvas>&& canvas) :Scene(std::move(canvas)) {}
        virtual ~SampleScene() = default;
        virtual void onStart() override;
        virtual void onUpdate(IInput&);
    };

}//namespace

