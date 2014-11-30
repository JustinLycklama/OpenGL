#include "Light.h"


Light::Light(Asset* asset) : Instance (asset)
{
}

Light::~Light(void)
{
}

vec3 Light::getIntensities() {
	return intensities;
}

float Light::getAttenuation() {
	return attenuation;
}

float Light::getAmbientCoefficient() {
	return ambientCoefficient;
}

void Light::setIntensities(vec3 col) {
	intensities = col;
}

void Light::setAttenuation(float att) {
	attenuation = att;
}

void Light::setAmbientCoefficient(float amb) {
	ambientCoefficient = amb;
}

void Light::render(Program* program) {
	
	// Set Light
	GLint lightPos = program->getUniformLocation("light.position");
	GLint lightColor = program->getUniformLocation("light.intensities");
	GLint lightAtten = program->getUniformLocation("light.attenuation");
	GLint lightAmbient = program->getUniformLocation("light.ambientCoefficient");

	glUniform3fv(lightPos, 1, glm::value_ptr(getPosition()));
	glUniform3fv(lightColor, 1, glm::value_ptr(intensities));
	glUniform1f(lightAtten, attenuation);
	glUniform1f(lightAmbient, ambientCoefficient);
	
	Instance::render();
}
