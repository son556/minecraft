#include "pch.h"
#include "CascadeShadow.h"
#include "DepthMap.h"
#include "DeferredGraphics.h"
#include "ConstantBuffer.h"
#include "MapUtils.h"

CascadeShadow::CascadeShadow(
	DeferredGraphics* d_graphic, 
	UINT width, 
	UINT height,
	MapUtils* m_info
)
{
	this->d_graphic = d_graphic;
	this->m_info = m_info;
	this->depth_buffer = make_shared<DepthMap>(
		this->d_graphic->getDevice(),
		height, width);
	this->depth_buffer->setViewPort(width, height);
	MVP mvp;
	this->cbuffer = make_shared<ConstantBuffer>(
		this->d_graphic->getDevice(),
		this->d_graphic->getContext(),
		mvp
	);
}

ComPtr<ID3D11ShaderResourceView> CascadeShadow::getSRV()
{
	return this->depth_buffer->getShaderResourceView();
}

ComPtr<ID3D11DepthStencilView> CascadeShadow::getDSV()
{
	return this->depth_buffer->getDepthStencilView();
}

void CascadeShadow::setFrustumVertices(
	vec3 const& coord, 
	int vertex_idx, 
	int frustum_idx)
{
	this->frustum_vertices[frustum_idx][vertex_idx][0] = coord.x;
	this->frustum_vertices[frustum_idx][vertex_idx][1] = coord.y;
	this->frustum_vertices[frustum_idx][vertex_idx][2] = coord.z;
}



void CascadeShadow::updateCBuffer(
	Mat const& cam_view,
	Mat const& cam_proj
)
{
	float x = 0;
	float y = 0;
	float z = 0;
	Mat inv_view = cam_view;
	inv_view = XMMatrixLookToLH(vec3(0.5, 15, 0.5),
		vec3(1, 0, 0), vec3(0, 1, 0));
	inv_view = inv_view.Invert();
	vector<vec4> coord;
	coord.resize(8);
	vec4 mid = vec4(0, 0, 0, 0);
	for (int i = 0; i < 2; i++) { // world space로 절두체 좌표값 저장
		for (int j = 0; j < 4; j++) {
			x = this->frustum_vertices[i][j][0];
			y = this->frustum_vertices[i][j][1];
			z = this->frustum_vertices[i][j][2];
			vec4 pos = XMVector4Transform(vec4(x, y, z, 1), inv_view);
			mid += pos;
			coord[i * 4 + j] = pos;
		}
	}
	mid /= 8; // 절두체의 중점(world space)
	float w = vec4::Distance(coord[4], coord[5]);
	vec3 lp = this->m_info->directional_light_pos;
	float flip = 1;
	// 태양의 절두체의 중점보다 왼쪽의 경우(부호 반전)
	if (lp.x < mid.x)
		flip = -1;
	w *= flip;
	float t = abs(lp.y / lp.x);
	vec3 light_pos = vec3(mid.x, mid.y, mid.z) - this->m_info->light_dir;
	//light_pos.z = mid.z;
	vec3 up = vec3(0, 1, 0);
	// 태양의 절두체의 중점보다 왼쪽의 경우(좌우 반전 방지)
	if (light_pos.x < mid.x + 0.00001)
		up = vec3(-1, 0, 0);
	vec3 m = vec3(mid.x, mid.y, mid.z);
	Mat light_view = XMMatrixLookToLH(light_pos, this->m_info->light_dir, up);
	this->mvp.view = light_view.Transpose();

	float max_x;
	float min_x;
	float max_y;
	float min_y;
	float max_z;
	float min_z;
	for (int i = 0; i < 8; i++) {
		vec4 trf = XMVector4Transform(coord[i], light_view);
		if (i == 0) {
			max_x = trf.x;
			min_x = max_x;
			max_y = trf.y;
			min_y = max_y;
			max_z = trf.z;
			min_z = max_z;
		}
		else {
			max_x = max(max_x, trf.x);
			min_x = min(min_x, trf.x);
			max_y = max(max_y, trf.y);
			min_y = min(min_y, trf.y);
			max_z = max(max_z, trf.z);
			min_z = min(min_z, trf.z);
		}
	}
	float z_mul = 10.f;

	// 절두체를 감싸는 상자의 near_z 와 max_z 간격을 넓혀 다른 상자와 overlap되게 함
	if (min_z < 0)
		min_z *= z_mul;
	else
		min_z /= z_mul;
	if (max_z < 0)
		max_z /= z_mul;
	else
		max_z *= z_mul;
	this->mvp.proj = XMMatrixOrthographicOffCenterLH(min_x,
		max_x, min_y, max_y, 0.0, max_z);
	/*this->mvp.proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(120),
		800.f / 650.f, 0.1, 1000);*/
	this->mvp.proj = this->mvp.proj.Transpose();
	this->cbuffer->update(this->mvp);
}

shared_ptr<ConstantBuffer> CascadeShadow::getCBuffer()
{
	return this->cbuffer;
}

MVP const& CascadeShadow::getMVP()
{
	return this->mvp;
}

D3D11_VIEWPORT CascadeShadow::getViewPort()
{
	return this->depth_buffer->getViewPort();
}
