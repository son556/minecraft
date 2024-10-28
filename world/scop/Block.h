#pragma once

#include "WorldUtils.h"

namespace Block {
	inline void addBlockFacePosAndTex(
		vec3 const& start_pos,
		int dir,
		float x,
		float y,
		float z,
		int type,
		vector<VertexGeo>& vertices
	)
	{
		static vector<vec3> sample_pos = {
			// top
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, -0.5f},

			// bottom
			{-0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, +0.5f},
			{-0.5f, -0.5f, +0.5f},

			// front
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},

			// back
			{-0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},

			// left
			{-0.5f, -0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},

			// right
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, +0.5f}
		};
		static vector<vec2> sample_uv = {
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
			{0.f, 1.f},

			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{1.f, 1.f},
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},

			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{1.f, 1.f},
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f}
		};
		static vector<vec3> normals = {
			{0, 1, 0},
			{0, -1, 0},
			{0, 0, -1},
			{0, 0, 1},
			{-1, 0, 0},
			{1, 0, 0}
		};
		VertexGeo vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;
		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.normal = normals[dir];
			vertex.uv = sample_uv[i];
			vertex.type = type;
			vertex.dir = dir;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFacePosAndTex(
		vec3 const& start_pos,
		float x,
		float y,
		float z,
		int dir,
		int shadow_flag,
		vector<VertexShadow>& vertices
	)
	{
		static vector<vec3> sample_pos = {
			// top
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, -0.5f},

			// bottom
			{-0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, +0.5f},
			{-0.5f, -0.5f, +0.5f},

			// front
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},

			// back
			{-0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},

			// left
			{-0.5f, -0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},

			// right
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, +0.5f}
		};
		static vector<vec2> sample_uv = {
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
			{0.f, 1.f},

			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{1.f, 1.f},
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},

			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},

			{1.f, 1.f},
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f}
		};
		static vector<vec3> normals = {
			{0, 1, 0},
			{0, -1, 0},
			{0, 0, -1},
			{0, 0, 1},
			{-1, 0, 0},
			{1, 0, 0}
		};
		VertexShadow vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;
		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.shadow_flag = shadow_flag;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFaceIndices(
		uint32 start, 
		vector<uint32>& indices
	)
	{
		indices.push_back(start);
		indices.push_back(start + 1);
		indices.push_back(start + 2);
		indices.push_back(start);
		indices.push_back(start + 2);
		indices.push_back(start + 3);
	}

	inline void makeCubeMap(
		float const& scale,
		vector<VertexDefer>& vertices,
		vector<uint32>& indices
	)
	{
		vector<vec3> positions;
		vector<vec2> texcoords; // 텍스춰 좌표

		// 윗면
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 아랫면
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 앞면
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 뒷면
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 왼쪽
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 오른쪽
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		VertexDefer vertex;
		for (int i = 0; i < positions.size(); i++) {
			vertex.pos = positions[i];
			vertex.uv = texcoords[i];
			vertices.push_back(vertex);
		}
		indices = {
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  6,  4,  6,  7,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼쪽
		20, 21, 22, 20, 22, 23  // 오른쪽
		};
		reverse(indices.begin(), indices.end());
	}
}