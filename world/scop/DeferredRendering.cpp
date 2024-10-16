#include "pch.h"
#include "DeferredRendering.h"
#include "MapUtils.h"
#include "DeferredGraphics.h"
#include "Graphics.h"
#include "InputLayout.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "Texture.h"

struct defferConst {
	int idx;
	vec3 cam_pos;
	Mat proj;
};

DeferredRendering::DeferredRendering(
	MapUtils* minfo,
	DeferredGraphics* defer_graphic
)
	: m_info(minfo), s_render(minfo, defer_graphic), 
	g_render(minfo, defer_graphic), ssao(defer_graphic->getDevice())
{
	this->d_graphic = defer_graphic;
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"DeferredVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"DeferredPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		layout.layout_deferred.data(),
		layout.layout_deferred.size(),
		this->vertex_shader->getBlob()
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
	vector<VertexDeffer> vertices;
	vector<uint32> indices;
	VertexDeffer v_deff;
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
	this->vbuffer = make_shared<Buffer<VertexDeffer>>(
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
	this->blend_state = make_shared<BlendState>(device);
	this->texture = make_shared<Texture>(
		device,
		L"random_texture.png"
	);
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = this->m_info->width / 2.0f;
	this->view_port.Height = this->m_info->height / 2.0f;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}

DeferredRendering::~DeferredRendering()
{
}

void DeferredRendering::Render(
	Mat const& cam_view, 
	Mat const& cam_proj, 
	vec3 const& cam_pos,
	Mat const& s_view, 
	Mat const& s_proj
)
{
	this->g_render.render(cam_view, cam_proj, cam_pos);
	this->s_render.render(s_view, s_proj);
	this->setPipe();
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	defferConst dc;
	dc.idx = 1;
	dc.cam_pos = cam_pos;
	dc.proj = cam_proj.Transpose();
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		this->d_graphic->getContext(),
		dc
	);
	context->PSSetConstantBuffers(0, 1, cbuffer.getComPtr().GetAddressOf());
	this->d_graphic->renderBegin();
	//this->d_graphic->setViewPort(this->view_port);
	for (int i = 0; i < 3; i++) {
		context->PSSetShaderResources(
			i,
			1,
			this->g_render.getSRV(i).GetAddressOf()
		);
	}
	context->PSSetShaderResources(
		3,
		1,
		this->s_render.getSRV().GetAddressOf()
	);
	context->PSSetShaderResources(
		4,
		1,
		this->s_render.getDepthSRV().GetAddressOf()
	);
	context->PSSetShaderResources(
		5,
		1,
		this->ssao.random_vec_SRV.GetAddressOf()
	);
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0,
		0
	);
	this->d_graphic->renderEnd();
}

void DeferredRendering::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = 
		this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(
		0,
		1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->PSSetSamplers(
		0,
		1,
		this->sampler_state->getComPtr().GetAddressOf()
	);
	context->OMSetBlendState(
		this->blend_state->getComPtr().Get(),
		this->blend_state->getBlendFactor(),
		this->blend_state->getSampleMask()
	);
	ConstantBuffer cbuffer(
		this->d_graphic->getDevice(),
		context,
		this->ssao.mOffsets
	);
	context->PSSetConstantBuffers(1, 1, cbuffer.getComPtr().GetAddressOf());
}


