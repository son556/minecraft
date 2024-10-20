#include "pch.h"
#include "DeferredRendering.h"
#include "MapUtils.h"
#include "DeferredGraphics.h"

DeferredRendering::DeferredRendering(
	MapUtils* minfo,
	DeferredGraphics* defer_graphic
)
	: m_info(minfo), s_render(minfo, defer_graphic), 
	g_render(minfo, defer_graphic), ssao_render(defer_graphic, 
		minfo->width, minfo->height)
{
	this->d_graphic = defer_graphic;
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
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	this->d_graphic->renderBegin();
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
	this->ssao_render.render(cam_proj);
	this->d_graphic->renderEnd();
}

