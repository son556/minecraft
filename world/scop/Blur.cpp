#include "pch.h"
#include "Blur.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "ConstantBuffer.h"
#include "Buffer.h"


Blur::Blur(DeferredGraphics* graphic, UINT width, UINT height)
{
	this->d_graphic = graphic;
	this->width = width;
	this->height = height;
	ComPtr<ID3D11Device> device = graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"SunMoonBlurVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_deferred.data(),
		InputLayouts::layout_deferred.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SunMoonBlurPS.hlsl",
		"main",
		"ps_5_0"
	);
	vector<vec3> sample_pos = {
		// front
		{-1.f, -1.f, 0.f},
		{-1.f, +1.f, 0.f},
		{+1.f, +1.f, 0.f},
		{+1.f, -1.f, 0.f},
	};
	vector<vec2> sample_uv = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},
	};
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	VertexDefer v_deff;
	for (int i = 0; i < 4; i++) {
		v_deff.pos = sample_pos[i];
		v_deff.uv = sample_uv[i];
		vertices.push_back(v_deff);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->d_buffer_x = make_shared<DeferredBuffer>(1);
	this->d_buffer_x->setRTVsAndSRVs(
		device,
		this->width,
		this->height
	);
	this->d_buffer_y = make_shared<DeferredBuffer>(1);

}

ComPtr<ID3D11ShaderResourceView> Blur::getSRV()
{
	return this->y_srv;
}

void Blur::render(
	ComPtr<ID3D11ShaderResourceView> input, 
	int cnt
)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
}

void Blur::computeShaderBarrier()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	ID3D11ShaderResourceView* reset_srv = nullptr;
	context->CSSetShaderResources(0, 1, &reset_srv);
	ID3D11UnorderedAccessView* reset_uav = nullptr;
	context->CSSetUnorderedAccessViews(0, 1, &reset_uav, nullptr);
}
