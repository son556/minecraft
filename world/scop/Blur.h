#pragma once

class DeferredGraphics;
class ComputeShader;

class Blur
{
public:
	Blur(DeferredGraphics* graphic, UINT width, UINT height);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void render(ComPtr<ID3D11ShaderResourceView> input, int cnt);

private:
	ComPtr<ID3D11ShaderResourceView> x_srv;
	ComPtr<ID3D11ShaderResourceView> y_srv;
	ComPtr<ID3D11UnorderedAccessView> x_uav;
	ComPtr<ID3D11UnorderedAccessView> y_uav;

private:
	DeferredGraphics* d_graphic;
	shared_ptr<ComputeShader> compute_shader;
};

