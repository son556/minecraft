#pragma once

#include "Map.h"

// test

class Terrain
{
public:
	Terrain(int size_w, int size_h, 
		HWND hwnd, UINT width, 
		UINT height, int fov_chunk, 
		int thread_cnt
	);
	~Terrain();
	void setSightChunk(int cnt);
	void userPositionCheck(float x, float z);
	int16 getHeight(float x, float z) const;
	void putBlock(vec3 const& ray_pos, vec3 const& ray_dir, int type);
	void deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir);
	pair<int, int> getBlock(float x, float y, float z);

public: // test
	void testClickLightBlock(vec3 const& ray_pos, 
		vec3 const& ray_dir);

public: // temp
	shared_ptr<Graphics> getGraphic();

public:
	void setRenderPipeLine(int flag);
	void DepthRender();
	void Render(
		Mat const& cam_view, 
		Mat const& cam_proj, 
		vec3 const& cam_pos
	);
	void setRender(int depth_flag);	

private:
	shared_ptr<Map> m_manager;
	shared_ptr<Graphics> graphic;
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;
};

