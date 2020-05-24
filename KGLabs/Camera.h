#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/polar_coordinates.hpp>
#include <GLM/gtx/euler_angles.hpp>
class Camera
{
public:
	// Constructors
	Camera();
	Camera(glm::vec3 position);
	Camera(float x, float y, float z);

	// Get and set camera pos
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

	// Move and rotate
	void rotate(float vertical, float horizontal);
	void zoomInOut(float distance);
	void move(float dx, float dz);

	// Get phi and theta
	glm::vec2 getAngles();

	// Get view matrix
	glm::mat4& getViewMatrix();

	// Set and get projection matrix
	void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	glm::mat4& getProjectionMatrix();

	// Get vectors
	glm::vec3 getForwardVector() { return forward; }
	glm::vec3 getRightVector() { return right; }
	glm::vec3 getUpVector() { return up; }

	// Default destructor
	~Camera() = default;

private:
	// Fields
	glm::vec3 eyePos;	    // x y z
	glm::vec3 spherical;	// r phi theta
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 lookAtPoint;  // pivot point
	glm::mat4 view;			// view matrix
	glm::mat4 projection;	// projection matrix

private:
	// Inner methods
	void calcSpherical();
	void update();
	void debug();
};