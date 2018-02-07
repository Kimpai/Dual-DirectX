#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <VertexTypes.h>
#include <wrl.h>

#include "GoodaModel.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class Terrain : public Model
{
public:
	Terrain(int, int);
	~Terrain();

private:
	void InitializeBuffers(ID3D12Device*, ID3D12GraphicsCommandList*);
	void RenderBuffers(ID3D12GraphicsCommandList*, int);
	void ShutdownBuffers();

	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	ComPtr<ID3D12Resource> m_vertexBufferUploadHeap;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	ComPtr<ID3D12Resource> m_indexBufferUploadHeap;

	int m_height, m_width;
	int m_terrainIndices;
};