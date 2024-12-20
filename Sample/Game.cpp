//
// Game.cpp
//

#include "pch.h"
#include <array>
#include <filesystem>
#include "Game.h"
#include "PrimitiveBatch.h"
#include "CommonStates.h"
#include "ResourceUploadBatch.h"
#include "Sample_0_CreateAndBind.h"
#include <LMotion.h>

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace LitMotionCpp;
using namespace LitMotionCpp::Sample;

using Microsoft::WRL::ComPtr;

enum Descriptors
{
    Font,
    Count,
};

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
    m_deviceResources->RegisterDeviceNotify(this);

    m_quadPositions.fill(VertexPositionColor{});
}

Game::~Game()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    m_resourceDescriptors = std::make_unique<DescriptorHeap>(m_deviceResources->GetD3DDevice(), Descriptors::Count);

    //  create spritefont file path
    std::array<CHAR, MAX_PATH> pathBuffer;
    ::GetModuleFileNameA(nullptr, pathBuffer.data(), static_cast<DWORD>(pathBuffer.size()));
    std::filesystem::path path{ pathBuffer.data() };
    path.remove_filename();
    path /= "sample.spritefont";

    //  upload resource
    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());
    ResourceUploadBatch resourceUpload(m_deviceResources->GetD3DDevice());
    resourceUpload.Begin();
    //  load spritefont
    m_spriteFont = std::make_unique<SpriteFont>(m_deviceResources->GetD3DDevice(), resourceUpload,
        path.c_str(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Font),
        m_resourceDescriptors->GetGpuHandle(Descriptors::Font));
    //  create SpriteBatch
    SpriteBatchPipelineStateDescription spd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDevice(), resourceUpload, spd);
    auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
    uploadResourcesFinished.wait();

    //  create PrimitiveBatch
    m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_deviceResources->GetD3DDevice());

    EffectPipelineStateDescription pd(
        &VertexPositionColor::InputLayout,
        CommonStates::Opaque,
        CommonStates::DepthDefault,
        CommonStates::CullNone,
        rtState);

    m_primitiveEffect = std::make_unique<BasicEffect>(m_deviceResources->GetD3DDevice(), EffectFlags::VertexColor, pd);
    GetDefaultSize(width, height);
    m_primitiveEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, static_cast<float>(width), static_cast<float>(height), 0, 0, 1));

    m_createAndBind = std::make_unique<Sample_0_CreateAndBind>();
    m_createAndBind->onStart();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    //float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    MotionDispatcher::setTime(timer.GetTotalSeconds());
    MotionDispatcher::update();

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.
    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);
    m_spriteBatch->SetViewport(m_deviceResources->GetScreenViewport());

    //  Draw Scene
    m_createAndBind->onDraw(*this);

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::White, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

#pragma region IInput
bool Game::pressedUp()
{
    return false;
}

bool Game::pressedDown()
{
    return false;
}
bool Game::pressedSpace()
{
    return false;
}
#pragma endregion

#pragma region IRenderer
void Game::drawSprite(Sprite& sprite)
{
    constexpr float width = 32.0f;
    constexpr float height = 32.0f;

    XMFLOAT3 position{ sprite.X-width/2.0f,sprite.Y-height/2.0f,0.0f };
    XMFLOAT4 color{ sprite.Color.x,sprite.Color.y,sprite.Color.z,sprite.Color.w };

    m_quadPositions[0].position = position;
    m_quadPositions[0].color = color;

    position.x += width;
    m_quadPositions[1].position = position;
    m_quadPositions[1].color = color;

    position.y += height;
    m_quadPositions[2].position = position;
    m_quadPositions[2].color = color;

    position.x -= width;
    m_quadPositions[3].position = position;
    m_quadPositions[3].color = color;

    auto commandList = m_deviceResources->GetCommandList();
    m_primitiveEffect->Apply(commandList);

    m_primitiveBatch->Begin(commandList);
    m_primitiveBatch->DrawQuad(m_quadPositions[0], m_quadPositions[1], m_quadPositions[2], m_quadPositions[3]);
    m_primitiveBatch->End();
}

void Game::drawText(float x, float y, const char* text)
{
    auto commandList = m_deviceResources->GetCommandList();

    m_spriteBatch->Begin(commandList);
    m_spriteFont->DrawString(m_spriteBatch.get(), text, XMFLOAT2(x, y),Colors::Black);
    m_spriteBatch->End();
}
#pragma endregion
