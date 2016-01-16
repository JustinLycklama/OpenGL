#include "StdAfx.h"

using namespace std;

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 1.0f);
    horizontalAngle = 0.0f;
    verticalAngle = 0.0f;
    fieldOfView = 50.0f;
    nearPlane = 0.5f;
    farPlane = 100.0f;
    aspectRatio = 4.0f/3.0f;

	maxVertAngle = 85.0f;
}

Camera::~Camera() {

}

void Camera::render(Program* program) {
	GLint cameraMatrix =  program->getUniformLocation("camera");
	glUniformMatrix4fv(cameraMatrix, 1, false, glm::value_ptr(matrix()));

	GLint cameraPos =  program->getUniformLocation("cameraPosition");
	glUniform3fv(cameraPos, 1, glm::value_ptr(getPosition()));
}

/* Getters */
glm::mat4 Camera::matrix() const {
    glm::mat4 camera = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    camera *= orientation();
    camera = glm::translate(camera, -position);
    return camera;
}

glm::mat4 Camera::orientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, verticalAngle, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, horizontalAngle, glm::vec3(0,1,0));
    return orientation;
}

GLfloat Camera::getFieldOfView() const {
	return fieldOfView;
}

vec3 Camera::getPosition() {
	return position;
}

vec3 Camera::forward() {
	vec4 forward = inverse(orientation()) * vec4(0, 0, -1, 1);
	return vec3(forward);
}

vec3 Camera::right() {
	vec4 forward = inverse(orientation()) * vec4(1, 0, 0, 1);
	return vec3(forward);
}

vec3 Camera::up() {
	vec4 forward = inverse(orientation()) * vec4(0, 1, 0, 1);
	return vec3(forward);
}


/* Setters */
void Camera::setPosition(vec3 pos) {
	position = pos;
}

void Camera::offsetPosition(const vec3& offset) {
	position += offset;
}

void Camera::offsetOrientation(float up, float right) {
	horizontalAngle += right;
	verticalAngle += up;
	
	if(horizontalAngle < 0.0f)
		horizontalAngle += 360.0f;
	if(horizontalAngle > 360.0f)
		horizontalAngle -= 360.0f;

	if(verticalAngle > maxVertAngle)
		verticalAngle = maxVertAngle;
	else if(verticalAngle < -maxVertAngle)
		verticalAngle = -maxVertAngle;
}

void Camera::setAspectRatio(GLfloat ar) {
	aspectRatio = ar;
}

void Camera::setFieldOfView(GLfloat FoV) {
	fieldOfView = FoV;
}