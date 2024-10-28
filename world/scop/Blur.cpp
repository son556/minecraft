#include "pch.h"
#include "Blur.h"
#include "DeferredGraphics.h"
#include "ComputeShader.h"

Blur::Blur(DeferredGraphics* graphic, UINT width, UINT height)
{
	ComPtr<ID3D11Device> device = graphic->getDevice();
	ComPtr<ID3D11Texture2D> texture_x, texture_y;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
		D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;
	HRESULT hr = device->CreateTexture2D(&desc, 
		nullptr, texture_x.GetAddressOf());
	CHECK(hr);
	hr = device->CreateTexture2D(&desc, 
		nullptr, texture_y.GetAddressOf());
	CHECK(hr);

	// 혹시 안된다면 desc를 설정안해서?
	hr = device->CreateShaderResourceView(texture_x.Get(), nullptr,
		this->x_srv.GetAddressOf());
	CHECK(hr);
	hr = device->CreateUnorderedAccessView(texture_x.Get(), nullptr,
		this->x_uav.GetAddressOf());
	CHECK(hr);

	hr = device->CreateShaderResourceView(texture_y.Get(), nullptr,
		this->y_srv.GetAddressOf());
	CHECK(hr);
	hr = device->CreateUnorderedAccessView(texture_y.Get(), nullptr,
		this->y_uav.GetAddressOf());
	CHECK(hr);
	this->compute_shader = make_shared<ComputeShader>(
		device,
		L"BlurCS.hlsl",
		"main",
		"cs_5_0"
	);
}

ComPtr<ID3D11ShaderResourceView> Blur::getSRV()
{
	return this->y_srv;
}

void Blur::render(ComPtr<ID3D11ShaderResourceView> input, int cnt)
{
}
