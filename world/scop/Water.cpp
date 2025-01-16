#include "pch.h"
#include "Water.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "RasterizerState.h"
#include "MapUtils.h"
#include "Chunk.h"

Water::Water(DeferredGraphics* d_graphic, MapUtils* m_info)
	: d_graphic(d_graphic), m_info(m_info)
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"WaterVS.hlsl",
		"main",
		"vs_5_0"
	);
	//this->input_layout = make_shared<InputLayout>
}
