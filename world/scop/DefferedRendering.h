#pragma once

enum class RenderTarget {
	NORMAL,
	POSITION,
	COLOR
};

class DefferedRendering
{
public:
	DefferedRendering(
		HWND hwnd, 
		UINT width, 
		UINT height,
		int rtv_num,
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context
	);
	~DefferedRendering();
	void renderBegin();
	ComPtr<ID3D11Device> getDevice();
	ComPtr<ID3D11DeviceContext> getContext();
	ComPtr<ID3D11RenderTargetView> getRTV(RenderTarget type);
	ComPtr<ID3D11ShaderResourceView> getSRV(RenderTarget type);
private:
	void createSwapChain();
	void createRTVsAndSRVs();
	void createDepthStencilView();
	void setViewPort();

private:
	HWND hWnd;
	UINT width;
	UINT height;
	int rtv_num;
	D3D11_VIEWPORT view_port = {0, };
	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<ID3D11RenderTargetView> RTVs[8];
	ComPtr<ID3D11ShaderResourceView> SRVs[8];
	ComPtr<ID3D11DepthStencilView> DSV;
	float clear_color[4] = { 0.f, 0.f, 0.f, 1.f };
};

