#pragma once
class TestCam
{
public:
	TestCam(
		float width, 
		float height,
		float fov,
		float cam_near, 
		float cam_far
	);
	~TestCam();
	void movePos(float x, float y, float z);
	void setDir(vec3 dir);
	void onMouseMove(WPARAM wparam, int mouse_x, int mouse_y);
	void update();
	MVP getViewProj();
	vec3 getPos();
	vec3 getDir();
	void setCursorInClient(HWND hwnd, int c_x, int c_y);
	void setNear(float cam_near);
	void setFar(float cam_far);
	void setFOV(float fov);
	float getFOV();
	float getNear();
	float getFar();
private:
	float w_width;
	float w_height;
	float cam_near;
	float cam_far;
	float fov;
	vec3 pos;
	vec3 dir;
	MVP mvp;
};

