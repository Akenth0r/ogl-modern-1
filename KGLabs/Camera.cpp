#include "Camera.h"

using namespace glm;

Camera::Camera()
{
	this->eyePos = vec3(0);
	this->spherical = vec3(0);
	this->lookAtPoint = vec3(0);
	this->view = identity<mat4>();
	this->projection = identity<mat4>();
}

Camera::Camera(vec3 position)
{
	this->eyePos = position;
	this->lookAtPoint = vec3(0);
	calcSpherical();
}

Camera::Camera(float x, float y, float z)
{
	this->eyePos = vec3(x, y, z);
	this->lookAtPoint = vec3(0);
	calcSpherical();
}

void Camera::setPosition(vec3 position)
{
	this->eyePos = position;
	calcSpherical();
	update();
}

vec3 Camera::getPosition()
{
	return this->eyePos;
}

void Camera::rotate(float vertical, float horizontal)
{
	this->spherical.y += horizontal;
	if (spherical.y < -360 || spherical.y > 360)
		spherical.y -= spherical.y;
	this->spherical.z = clamp<float>(spherical.z + vertical, 5.0f, 85.0f);
	update();
}

void Camera::zoomInOut(float distance)
{
	this->spherical.x = clamp<float>(this->spherical.x + distance, 3.0f, 120.0f);
	update();
}

void Camera::move(float dx, float dz)
{
	vec3 lookEye = lookAtPoint - eyePos;
	lookEye.y = 0;
	vec3 forward = normalize(lookEye);
	vec3 right = cross(vec3(0, 1, 0), forward);
	vec3 dpos = forward * dx + right * dz;
	this->eyePos += dpos;
	this->lookAtPoint += dpos;
	update();
}

void Camera::calcSpherical()
{
	float dist = distance(lookAtPoint, eyePos);

	vec3 projXY = vec3(eyePos.x, eyePos.y, 0);
	vec3 projZX = vec3(eyePos.x, 0, eyePos.z);
	this->spherical = vec3(
		dist,
		degrees((acos(dot(normalize(eyePos), normalize(projZX))))),			// PHI Y
		degrees(acos(dot(normalize(projXY), normalize(projZX))))			// THETA Z
	);
}

vec2 Camera::getAngles()
{
	return vec2(spherical.y, spherical.z);
}

mat4& Camera::getViewMatrix()
{
	return this->view;
}

void Camera::setProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	this->projection = glm::perspective(fovy, aspect, zNear, zFar);
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projection;
}

void Camera::update()
{
	mat4 R = yawPitchRoll<float>(-radians(spherical.y), -radians(spherical.z), 0);
	vec4 T = R * vec4(0, 0, spherical.x, 0);
	eyePos = lookAtPoint + vec3(T);
	this->view = lookAt(this->eyePos, this->lookAtPoint, vec3(0, 1, 0));
	debug();
}

void Camera::debug()
{

	#ifdef _DEBUG
	vec3 lookEye = lookAtPoint - eyePos;
	lookEye.y = 0;
	vec3 forward = normalize(lookEye);
	vec3 forward_view = normalize(vec3(view[2]));
	forward.y = 0;
	printf(
		"pivot: (%f, %f, %f)\n"
		"pos: (%f, %f, %f)\n"
		"spherical: (%f, %f, %f)\n"
		"forward: (%f, %f, %f)\n"
		"length-forward: %f\n"
		"forward-view: (%f, %f, %f)\n"
		"view-length: %f\n\n",
		lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
		eyePos.x, eyePos.y, eyePos.z,
		spherical.x, spherical.y, spherical.z,
		forward.x, forward.y, forward.z,
		length(forward),
		forward_view.x, forward_view.y, forward_view.z,
		length(forward_view)
		);
	#endif
}