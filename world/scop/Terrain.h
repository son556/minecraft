#pragma once

#include "PerlinNoise.h"

class Graphics;
class RasterizerState;
class TextureArray;
class BlendState;
class SamplerState;
class Chunk;

class Terrain
{
public:
	Terrain(HWND hwnd, uint32 width, uint32 height);
	~Terrain();
	void setViewAndProj(Mat const& view, Mat const& proj);
	void setRender();
	void Render();
	void setCam(Mat view, Mat proj);
	void createHeightMap();
	void terrainsetVerticesAndIndices();

private:
	vector<pair<int, int>> coordinateToIndex(float x, float y, float z) const;

private:
	Mat view;
	Mat proj;
	map<vec3, uint32> object_book;
	shared_ptr<Chunk> terrain[30][30];
	int16 height_map[480][480];
	PerlinNoise perlin_noise;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;

	int size_w = 8;
	int size_h = 8;
};

