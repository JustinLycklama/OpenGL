#include <glm/gtc/matrix_access.hpp>

#include "Instance.h"

Instance::Instance(Asset* ast)
{
	asset = ast;
}


Instance::~Instance(void)
{
}

void Instance::update(float secondsElapsed) {
	rotate(vec3(0, 0, 1), 1);
}

void Instance::render() {
	if(asset != NULL) {
		GLint modelPos =  asset->program->getUniformLocation("model");
		glUniformMatrix4fv(modelPos, 1, false, glm::value_ptr(getTransform()));

		asset->render();
	}
}

/* Getters */

mat4 Instance::getTransform() {
	return translated * rotated * scaled;

}

vec3 Instance::getPosition() {
	return vec3(getTransform() * vec4(0, 0, 0, 1));
}

vec3 Instance::getDirection() {
	mat4 transform = getTransform();
	return -vec3(glm::column(transform, 2));
}

mat4 Instance::getTranslation() {
	return translated;
}

mat4 Instance::getRotation() {
	return rotated;
}

mat4 Instance::getScale() {
	return scaled;
}

/* Setters */

void Instance::copyTransform(Instance* other) {
	translated = other->getTranslation();	
	rotated = other->getRotation();
	scaled = other->getScale();
}

void Instance::copyTransform(Camera* camera) {
	translated = glm::translate(mat4(), camera->getPosition());
	rotated = glm::inverse(camera->orientation());
}

void Instance::translate(vec3 trans) {
	translated = glm::translate(translated, trans);
}

void Instance::rotate(vec3 axis, float angle) {
	rotated = glm::rotate(rotated, angle, axis);
}

void Instance::scale(vec3 sc) {
	scaled = glm::scale(scaled, sc);
}


void Instance::setAsset(Asset* ast) {
	asset = ast;
}