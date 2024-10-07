#include "pch.h"
#include "DefferedRendering.h"

DefferedRendering::DefferedRendering(
	HWND hwnd, 
	UINT width, 
	UINT height,
	int rtv_num,
	ComPtr<ID3D11Device> device,
	ComPtr<ID3D11DeviceContext> context
) : hWnd(hwnd), width(width), height(height)
{
	if (rtv_num > 8) {
		cout << "wrong render target number" << endl;
		exit(-1);
	}
	this->rtv_num = rtv_num;
	this->device = device;
	this->context = context;
	this->createSwapChain();
	this->createRTVsAndSRVs();
	this->createDepthStencilView();
}

DefferedRendering::~DefferedRendering()
{
}

void DefferedRendering::renderBegin()
{
	this->context->OMSetRenderTargets(
		this->rtv_num, this->RTVs[0].GetAddressOf(),
		this->DSV.Get()
	);
	for (int i = 0; i < this->rtv_num; i++) {
		this->context->ClearRenderTargetView(
			this->RTVs[i].Get(), this->clear_color);
	}
	this->context->ClearDepthStencilView(
		this->DSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		0
	);
	this->context->RSSetViewports(1, &(this->view_port));
}

ComPtr<ID3D11Device> DefferedRendering::getDevice()
{
	return this->device;
}

ComPtr<ID3D11DeviceContext> DefferedRendering::getContext()
{
	return this->context;
}

ComPtr<ID3D11RenderTargetView> DefferedRendering::getRTV(RenderTarget type)
{
	return this->RTVs[static_cast<int>(type)];
}

ComPtr<ID3D11ShaderResourceView> DefferedRendering::getSRV(RenderTarget type)
{
	return this->SRVs[static_cast<int>(type)];
}

void DefferedRendering::createSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferDesc.Width = this->width;
	desc.BufferDesc.Height = this->height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_SHADER_INPUT |
		DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.OutputWindow = this->hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HREFTYPE hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		this->swap_chain.GetAddressOf(),
		nullptr, // device 안만들고 기존 것 써도 되는 지 시험
		nullptr,
		nullptr // context 안만들고 기존 것 써도 되는 지 시험
	);
	CHECK(hr);
}

void DefferedRendering::createRTVsAndSRVs()
{
	D3D11_TEXTURE2D_DESC t_desc;
	ZeroMemory(&t_desc, sizeof(t_desc));
	t_desc.Width = this->width;
	t_desc.Height = this->height;
	t_desc.MipLevels = 1;
	t_desc.ArraySize = 1;
	t_desc.Usage = D3D11_USAGE_DEFAULT;
	t_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	t_desc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	t_desc.CPUAccessFlags = 0;
	t_desc.MipLevels = 0;
	t_desc.SampleDesc.Count = 1;
	t_desc.SampleDesc.Quality = 0;
	HRESULT hr;
	ComPtr<ID3D11Texture2D> tex[5];
	for (int i = 0; i < this->rtv_num; i++) {
		hr = this->device->CreateTexture2D(&t_desc,
			nullptr, tex[i].GetAddressOf());
		CHECK(hr);
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = t_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	for (int i = 0; i < this->rtv_num; i++) {
		hr = this->device->CreateRenderTargetView(tex[i].Get(),
			&rtv_desc, this->RTVs[i].GetAddressOf());
		CHECK(hr);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = t_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	for (int i = 0; i < rtv_num; i++) {
		hr = this->device->CreateShaderResourceView(
			tex[i].Get(), &srv_desc, this->SRVs[i].GetAddressOf());
		CHECK(hr);
	}
}

void DefferedRendering::createDepthStencilView()
{
	ComPtr<ID3D11Texture2D> depth_texture;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = this->width;
	desc.Height = this->height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	HRESULT hr = this->device->CreateTexture2D(
		&desc, nullptr, depth_texture.GetAddressOf());
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = desc.Format;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = this->device->CreateDepthStencilView(
		depth_texture.Get(), &dsv_desc, this->DSV.GetAddressOf());
	CHECK(hr);
}

void DefferedRendering::setViewPort()
{
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = static_cast<float>(this->width);
	this->view_port.Height = static_cast<float>(this->height);
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;
}
