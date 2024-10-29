#pragma once

class DeferredGraphics;
class DeferredBuffer;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class Sun;
class Moon;
class Blur;

class SunMoon
{
public:
	SunMoon(
		DeferredGraphics* dgraphic, 
		UINT width,
		UINT height
	);
	void render(
		vec3 const& cam_pos, 
		Mat const& cam_view,
		Mat const& cam_proj,
		DeferredBuffer* dbuffer
	);

private:
	void setPipe();
	// 지표면 위의 달과 태양 중 하나의 위치 
	// (둘 다 지표면위면 태양의 위치)
	vec3 getLightPos();

private:
	shared_ptr<Sun> sun;
	shared_ptr<Moon> moon;
	shared_ptr<Blur> blur;
	vec3 sun_pos;
	vec3 moon_pos;
private:
	DeferredGraphics* d_graphic;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
private: // blur 용
	shared_ptr<InputLayout> b_input_layout;
	shared_ptr<VertexShader> b_vertexShader;
	shared_ptr<PixelShader> b_pixel_shader;
};

