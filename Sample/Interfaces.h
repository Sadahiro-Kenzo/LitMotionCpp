#pragma once
#include "memory"
#include "ICanvas.h"

namespace LitMotionCpp::Sample
{
	class IInput
	{
	public:
		virtual bool pressedUp() = 0;
		virtual bool pressedDown()=0;
		virtual bool pressedSpace() = 0;
		virtual bool pressedEscape() = 0;
	};

	class IScene
	{
	public:
		IScene() = default;
		virtual ~IScene() = default;
		virtual void onStart() = 0;
		virtual void onUpdate(IInput&) = 0;
		virtual void onEnd() = 0;
		virtual ICanvas& getCanvas() = 0;
		virtual const ICanvas& getCanvas() const = 0;
	};

	class Scene :public IScene
	{
	private:
		std::unique_ptr<ICanvas> m_canvas;
	public:
		Scene() = delete;
		Scene(std::unique_ptr<ICanvas>&& canvas) :m_canvas(std::move(canvas)) {}
		virtual ~Scene() = default;
		// IScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual ICanvas& getCanvas() override { return *m_canvas; }
		virtual const ICanvas& getCanvas() const override { return *m_canvas; }
	};

}//namespace
