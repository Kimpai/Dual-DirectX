#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dx12.h>
#include <wrl.h>
#include "colorshaderclass.h"
#include "frame.h"

using namespace Microsoft::WRL;

class Direct3DClass
{
public:
	Direct3DClass();
	Direct3DClass(const Direct3DClass&);
	~Direct3DClass();

	bool Initialize(int, int, HWND, bool, bool, float, float);
	void Shutdown();

	bool BeginScene(ColorShaderClass*);
	bool EndScene();
	bool CloseCommandList();
	bool ResetCommandList(ID3D12PipelineState*);
	bool ExecuteCommandList();
	ID3D12Device* GetDevice();
	ID3D12GraphicsCommandList* GetCommandList();
	int GetCurrentFrame();

private:
	bool WaitforFrameToFinish();

	bool m_vsync;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	char m_videoCardDescription[128];
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12DescriptorHeap> m_renderTargetViewDescHeap;
	ComPtr<ID3D12Resource> m_backBufferRenderTarget[frameBufferCount];
	unsigned int m_bufferIndex;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator[frameBufferCount];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12Fence> m_fence[frameBufferCount];
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue[frameBufferCount];
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_rect;
	int m_frameIndex;
	int m_videoCardMemory;
};
