#include "Instance.h"


Instance::Instance(Asset* ast)
{
	asset = ast;
}


Instance::~Instance(void)
{
}

void Instance::update(float secondsElapsed) {

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

/* Setters */

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