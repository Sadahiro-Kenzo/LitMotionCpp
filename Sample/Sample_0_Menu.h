#pragma once
#include <vector>
#include "Interfaces.h"

namespace LitMotionCpp::Sample
{
    class Sample_0_Menu : public Scene
    {
	private:
		int m_currentIndex = 0;
		std::vector<std::weak_ptr<ITextLabel>> m_menuItems;
	public:
		Sample_0_Menu(std::unique_ptr<ICanvas>&& canvas);

		// IScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void onStart() override;
		void onUpdate(IInput&) override;
		void onEnd() override;
	};
}//namespace

