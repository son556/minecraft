#pragma once

class DeferredGraphics;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class Sun;
class Moon;

class SunMoon
{
public:
	SunMoon(DeferredGraphics* dgraphic);
	void render(
		vec3 const& cam_pos, 
		Mat const& cam_view,
		Mat const& cam_proj
	);

private:
	void setPipe();
	// 지표면 위의 달과 태양 중 하나의 위치 
	// (둘 다 지표면위면 태양의 위치)
	vec3 getLightPos();

private:
	shared_ptr<Sun> sun;
	shared_ptr<Moon> moon;
	vec3 sun_pos;
	vec3 moon_pos;
private:
	DeferredGraphics* d_graphic;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
};

