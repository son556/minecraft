#include "pch.h"
#include "TestCam.h"
#include <algorithm>

TestCam::TestCam(
	float width, 
	float height, 
	float fov,
	float cam_near, 
	float cam_far
)
{
	this->w_width = width;
	this->w_height = height;
	this->fov = fov;
	this->setDir(vec3(0, 0, 1));
	this->mvp.view = Mat::Identity;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(fov),
		width / height,
		cam_near,
		cam_far
	);
	this->cam_near = cam_near;
	this->cam_far = cam_far;
}

TestCam::~TestCam()
{
}

void TestCam::movePos(float x, float y, float z)
{
	this->pos = vec3(x, y, z);
}

void TestCam::setDir(vec3 dir)
{
	this->dir = XMVector3Normalize(dir);
}

void TestCam::onMouseMove(WPARAM wparam, int mouse_x, int mouse_y)
{
	float x = mouse_x * 2.0 / this->w_width - 1.0;
	float y = -mouse_y * 2.0 / this->w_height + 1.0;
	if (x < -1.f)
		x = -1.f;
	if (x > 1.f)
		x = 1.f;
	if (y < -1.f)
		y = -1.f;
	if (y > 1.f)
		y = 1.f;
	vec4 pos = vec4(x, y, 0, 1) * this->cam_near;
	vec4 ppos = vec4(x, y, 1, 1) * this->cam_far;
	Mat r_mat = (this->mvp.view * this->mvp.proj).Invert();
	pos = vec4::Transform(pos, r_mat);
	ppos = vec4::Transform(ppos, r_mat);
	vec4 ddir = ppos - pos;
	this->dir = vec3(ddir.x, ddir.y, ddir.z);
	this->dir.Normalize();
	float t = this->dir.Cross(vec3(0, 1, 0)).Length();
	if (t < 0.0000001 && t > -0.0000001) {
		if (this->dir.y > 0)
			this->dir = XMVector3Normalize(vec3(0, 1, 0.001));
		else
			this->dir = XMVector3Normalize(vec3(0, -1, 0.001));
	}
	this->mvp.view = XMMatrixLookToLH(this->pos, this->dir, vec3(0, 1, 0));
}

void TestCam::setCursorInClient(HWND hwnd, int c_x, int c_y)
{
	POINT pt;
	pt.x = c_x;
	pt.y = c_y;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void TestCam::update()
{
	vec3 right_dir = vec3(0, 1, 0).Cross(this->dir);
	vec3 up_dir = this->dir.Cross(right_dir);
	vec3 move_dir = vec3(0, 0, 0);
	if (GetAsyncKeyState('A') & 0x8000)
		move_dir -= right_dir;
	if (GetAsyncKeyState('D') & 0x8000)
		move_dir += right_dir;
	if (GetAsyncKeyState('W') & 0x8000)
		move_dir += this->dir;
	if (GetAsyncKeyState('S') & 0x8000)
		move_dir -= this->dir;
	if (GetAsyncKeyState('Q') & 0x8000)
		move_dir += up_dir;
	if (GetAsyncKeyState('E') & 0x8000)
		move_dir -= up_dir;
	move_dir = XMVector3Normalize(move_dir) * 0.03f;
	this->pos += move_dir;
	this->mvp.view = XMMatrixLookToLH(this->pos, this->dir, vec3(0, 1, 0));
}

void TestCam::setNear(float cam_near)
{
	this->cam_near = cam_near;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

void TestCam::setFar(float cam_far)
{
	this->cam_far = cam_far;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

void TestCam::setFOV(float fov)
{
	this->fov = fov;
	this->mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToDegrees(this->fov),
		this->w_width / this->w_height,
		this->cam_near,
		this->cam_far
	);
}

float TestCam::getFOV()
{
	return this->fov;
}

float TestCam::getNear()
{
	return this->cam_near;
}

float TestCam::getFar()
{
	return this->cam_far;
}


MVP TestCam::getViewProj()
{
	return this->mvp;
}

vec3 TestCam::getPos()
{
	return this->pos;
}

vec3 TestCam::getDir()
{
	return this->dir;
}
