//
// Game.h
//

#pragma once

#include <array>
#include <memory>
#include "DeviceResources.h"
#include "StepTimer.h"
#include "GraphicsMemory.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "Effects.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "DescriptorHeap.h"
#include "Interfaces.h"

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify,LitMotionCpp::Sample::IInput, LitMotionCpp::Sample::IRenderer
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

    // IRenderer
    virtual void drawSprite(LitMotionCpp::Sample::Sprite& sprite) override;
    virtual void drawText(float x, float y, const char* text) override;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
    std::unique_ptr<DirectX::BasicEffect> m_primitiveEffect;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

    std::array<DirectX::VertexPositionColor,4> m_quadPositions;

    //  sample scene
    std::unique_ptr<LitMotionCpp::Sample::IScene> m_createAndBind;
};
