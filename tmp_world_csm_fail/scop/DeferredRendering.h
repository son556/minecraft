#pragma once

#include "ShadowRender.h"
#include "GeoRender.h"
#include "WorldUtils.h"
#include "BlendState.h"
#include "SsaoRender.h"
#include "SsaoBlur.h"
#include "PBR.h"

class DeferredGraphics;
class MapUtils;
template <typename T> class Buffer;
class Wallpaper;

class DeferredRendering
{
public:
	DeferredRendering(
		MapUtils* minfo,
		DeferredGraphics* defer_graphic
	);
	~DeferredRendering();
	void Render(
		Mat const& cam_view,
		Mat const& cam_proj,
		vec3 const& cam_pos,
		Mat const& s_view,
		Mat const& s_proj
	);
	
private:
	void setPipe();
	void setPBRShaderResources();
	void ssaoBlur(int cnt, 
		Mat const& proj,
		Mat const& view
	);

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	MapUtils* m_info = nullptr;
	DeferredGraphics* d_graphic;
	ShadowRender s_render;
	GeoRender g_render;
	SsaoRender ssao_render;
	SsaoBlur ssao_blur;
	PBR pbr;
	shared_ptr<Wallpaper> cube_map;
	D3D11_VIEWPORT view_port;

private:
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<InputLayout> input_layout;
};
