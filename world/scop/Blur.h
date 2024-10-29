#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
class DeferredBuffer;
class PixelShader;
class VertexShader;
class InputLayout;
class SamplerState;
template<typename T> class Buffer;

class Blur
{
public:
	Blur(DeferredGraphics* graphic, UINT width, UINT height);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void render(
		ComPtr<ID3D11ShaderResourceView> input, 
		int cnt
	);

private:
	void computeShaderBarrier();
private:
	ComPtr<ID3D11ShaderResourceView> x_srv;
	ComPtr<ID3D11ShaderResourceView> y_srv;
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	DeferredGraphics* d_graphic = nullptr;
	UINT width;
	UINT height;
	D3D11_VIEWPORT view_port = { 0, };
	shared_ptr<DeferredBuffer> d_buffer_x;
	shared_ptr<DeferredBuffer> d_buffer_y;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

