#pragma once

class MapUtils;
class DeferredGraphics;
class BlendState; // 바뀔 수 있음 나중에(ex OIT)
class TextureArray;
class SamplerState;
class VertexShader;
class PixelShader;
class InputLayout;
class RasterizerState;
class DeferredBuffer;
class HullShader;
class DomainShader;
class Texture;

class GeoRender
{
public:
	GeoRender(
		MapUtils* minfo,
		DeferredGraphics* dgraphic
	);
	~GeoRender();
	void render(
		Mat const& view,
		Mat const& proj,
		vec3 const& cam_pos
	);
	ComPtr<ID3D11ShaderResourceView> getSRV(int idx);

private:
	void setPipe();

private:
	MapUtils* m_info;
	DeferredGraphics* d_graphic;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<TextureArray> texture_array_color;
	shared_ptr<TextureArray> texture_array_normal;
	shared_ptr<TextureArray> texture_array_s;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<HullShader> hull_shader;
	shared_ptr<DomainShader> domain_shader;
};

