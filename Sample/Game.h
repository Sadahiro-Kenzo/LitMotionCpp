//
// Game.h
//

#pragma once

#include <array>
#include <memory>
#include <vector>
#include "DeviceResources.h"
#include "StepTimer.h"
#include "GraphicsMemory.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "Effects.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "DescriptorHeap.h"
#include "keyboard.h"
#include "Interfaces.h"

namespace LitMotionCpp::Sample
{
	class DX12Canvas;
}

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify,LitMotionCpp::Sample::IInput
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

    // IInput
    virtual bool pressedUp() override;
    virtual bool pressedDown() override;
    virtual bool pressedSpace() override;
	virtual bool pressedEscape() override;
private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void drawSprite(LitMotionCpp::Sample::DX12Canvas& canvas);
    void drawText(LitMotionCpp::Sample::DX12Canvas& canvas);

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    //std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
    //std::unique_ptr<DirectX::BasicEffect> m_primitiveEffect;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

    //std::array<DirectX::VertexPositionColor,4> m_quadPositions;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_circleTexture;

    //  sample scene
	size_t m_currentScene = 0;
    std::vector<std::unique_ptr<LitMotionCpp::Sample::IScene>> m_scenes;
};
