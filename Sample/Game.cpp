//
// Game.cpp
//

#include "pch.h"
#include <array>
#include <filesystem>
#include <SimpleMath.h>
#include "Game.h"
#include "PrimitiveBatch.h"
#include "CommonStates.h"
#include "ResourceUploadBatch.h"
#include "Sample_0_Menu.h"
#include "Sample_0_CreateAndBind.h"
#include "Sample_0_Easing.h"
#include "Sample_0_Loops.h"
#include "Sample_0_Delay.h"
#include "Sample_0_Callback.h"
#include "Sample_0_CancelAndComplete.h"
#include "BenchmarkScene.h"
#include "DX12Canvas.h"
#include <LMotion.h>
#include <WICTextureLoader.h>
#include <DirectXHelpers.h>
#include "Message.h"
#include "JobSystem.h"
#include "DirectXMathAnimationCurve.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace LitMotionCpp;
using namespace LitMotionCpp::Sample;

using Microsoft::WRL::ComPtr;

enum Descriptors
{
    Font,
	Circle,
    Count,
};

constexpr float VIEW_WIDTH = 800.0f;
constexpr float VIEW_HEIGHT = 600.0f;
constexpr float CAMERA_SIZE = 5.0f;
constexpr float PPUU = (VIEW_HEIGHT / 2.0f) / CAMERA_SIZE; // pixel per unity unit

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
    m_deviceResources->RegisterDeviceNotify(this);

    //m_quadPositions.fill(VertexPositionColor{});
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

	initializeJobSystem();
	initializeAnimationCurve();

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
    path /= "Assets";

    //  upload resource
    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());
    ResourceUploadBatch resourceUpload(m_deviceResources->GetD3DDevice());
    resourceUpload.Begin();
    //  load spritefont
	auto fontPath{ path };
    fontPath /= "open-sans.spritefont";
    m_spriteFont = std::make_unique<SpriteFont>(m_deviceResources->GetD3DDevice(), resourceUpload,
        fontPath.c_str(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Font),
        m_resourceDescriptors->GetGpuHandle(Descriptors::Font));
	//  load circle
	auto circlePath{ path };
	circlePath /= "circle.png";
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), resourceUpload, circlePath.c_str(),m_circleTexture.ReleaseAndGetAddressOf()));
    //  create SpriteBatch
    SpriteBatchPipelineStateDescription spd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDevice(), resourceUpload, spd);
    auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
    uploadResourcesFinished.wait();

    //  create PrimitiveBatch
    //m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_deviceResources->GetD3DDevice());

    EffectPipelineStateDescription pd(
        &VertexPositionColor::InputLayout,
        CommonStates::Opaque,
        CommonStates::DepthDefault,
        CommonStates::CullNone,
        rtState);

    //m_primitiveEffect = std::make_unique<BasicEffect>(m_deviceResources->GetD3DDevice(), EffectFlags::VertexColor, pd);
    //GetDefaultSize(width, height);
    //m_primitiveEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, static_cast<float>(width), static_cast<float>(height), 0, 0, 1));
    //m_primitiveEffect->SetProjection(XMMatrixOrthographicOffCenterRH(-6.67f, 6.67f, -5.0f, 5.0f, 0, 1));

	//  create circle texture descriptor
	CreateShaderResourceView(m_deviceResources->GetD3DDevice(), m_circleTexture.Get(), m_resourceDescriptors->GetCpuHandle(Descriptors::Circle));

	//  create keyboard
	m_keyboard = std::make_unique<DirectX::Keyboard>();

	//  create sample scene
    m_scenes.emplace_back(std::make_unique<Sample_0_Menu>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_CreateAndBind>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_Easing>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_Loops>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_Delay>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_Callback>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<Sample_0_CancelAndComplete>(std::make_unique<DX12Canvas>()));
    m_scenes.emplace_back(std::make_unique<BenchmarkScene>(std::make_unique<DX12Canvas>()));
    m_scenes[0]->onStart();
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

    while(!Message::IsEmpty())
    {
        const auto [message, param] = Message::Fetch();
        if (message == Message::Type::Select)
        {
            m_currentScene = static_cast<size_t>(param) + 1;
            m_scenes[m_currentScene]->onStart();
        }
        else if (message == Message::Type::Close)
        {
            m_currentScene = 0;
            m_scenes[m_currentScene]->onStart();
        }
    }
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    //float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    MotionDispatcher::setTime(timer.GetTotalSeconds());
    MotionDispatcher::update(MotionScheduler::getDefaultScheduler());

	m_keyboardTracker.Update(m_keyboard->GetState());
	m_scenes[m_currentScene]->onUpdate(*this);

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
	auto& canvas = static_cast<DX12Canvas&>(m_scenes[m_currentScene]->getCanvas());
	drawSprite(canvas);
	drawText(canvas);

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
	width = static_cast<int>(VIEW_WIDTH);
    height = static_cast<int>(VIEW_HEIGHT);
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
	return m_keyboardTracker.pressed.Up;
}

bool Game::pressedDown()
{
	return m_keyboardTracker.pressed.Down;
}
bool Game::pressedSpace()
{
	return m_keyboardTracker.pressed.Space;
}
bool Game::pressedEscape()
{
    return m_keyboardTracker.pressed.Escape;
}
#pragma endregion

void Game::drawSprite(DX12Canvas& canvas)
{
    XMFLOAT2 position;
    XMVECTOR color;

    auto matrix = SimpleMath::Matrix::CreateTranslation((VIEW_WIDTH/2.0f)/PPUU,(VIEW_HEIGHT/2.0f)/PPUU, 0.f);
	matrix *= SimpleMath::Matrix::CreateScale(PPUU, PPUU, 1.0f);
    
	const auto descriptor = m_resourceDescriptors->GetGpuHandle(Descriptors::Circle);
	const auto resourceSize = GetTextureSize(m_circleTexture.Get());
    const RECT sourceRect{ 0,0,static_cast<LONG>(resourceSize.x),static_cast<LONG>(resourceSize.y) };
    const XMFLOAT2 origin{ resourceSize.x*0.5f,resourceSize.y*0.5f };
	const float scale = 1.0f / resourceSize.y;  //  (1 unity unit) / (texture pixel per unit)

    auto commandList = m_deviceResources->GetCommandList();
	m_spriteBatch->Begin(commandList, SpriteSortMode_Deferred, matrix);
    for(auto sprite : canvas.GetSprites())
	{
        position.x = sprite->GetX();
        position.y = -sprite->GetY();
		color = XMLoadFloat4(sprite->DX12GetColor());
        m_spriteBatch->Draw(descriptor, resourceSize, position, &sourceRect, color, 0.0f, origin, scale);
	}
	m_spriteBatch->End();
}

void Game::drawText(DX12Canvas& canvas)
{
    auto commandList = m_deviceResources->GetCommandList();

    auto matrix = SimpleMath::Matrix::CreateTranslation(VIEW_WIDTH / 2.0f, VIEW_HEIGHT / 2.0f, 0.f);

    m_spriteBatch->Begin(commandList, SpriteSortMode_Deferred, matrix);
	XMFLOAT2 pos;
    XMVECTOR color;
    for(auto label : canvas.GetTextLabels())
    {
		pos.x = label->GetX() - (label->GetWidth() * label->GetPivotX());
        pos.y = label->GetY() + (label->GetHeight() * label->GetPivotY());

        const auto textSize = m_spriteFont->MeasureString(label->GetText());
        if(label->GetHorizontalAlign() != HorizontalAlign::Left)
        {
            if (label->GetHorizontalAlign() == HorizontalAlign::Center)
            {
				pos.x += (label->GetWidth() - XMVectorGetX(textSize)) / 2.0f;
            }
            else
			{
                pos.x += (label->GetWidth() - XMVectorGetX(textSize));
            }
        }
        if (label->GetVerticalAlign() != VerticalAlign::Top)
        {
            if (label->GetVerticalAlign() == VerticalAlign::Middle)
            {
                pos.y -= (label->GetHeight() - XMVectorGetY(textSize)) / 2.0f;
            }
            else
            {
                pos.y -= (label->GetHeight() - XMVectorGetY(textSize));
            }
        }

        color = XMLoadFloat4(label->DX12GetColor());
		pos.y = -pos.y;
        m_spriteFont->DrawString(m_spriteBatch.get(), label->GetText(), pos,color);
	}
    m_spriteBatch->End();
}

void Game::initializeJobSystem()
{
	JobSystem::initialize(4);

	ParallelJobScheduler::setExecuter([](IJobParallelFor& job, size_t size, size_t innerLoopBatchCount){
        auto handle=JobSystem::schedule([&job](size_t index) {
			    job.execute(static_cast<int>(index));
			}, size, innerLoopBatchCount);
		handle.wait();
		});
}

void Game::initializeAnimationCurve()
{
    AnimationCurve::setFactory([](const Keyframe* begin, const Keyframe* end, WrapMode preMode, WrapMode postMode) -> AnimationCurve* {
        return new DirectXMathAnimationCurve(begin,end,preMode,postMode);
		});
}

