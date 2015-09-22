#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "Light.h"

int Light::count = 0;

Light::Light(Asset* asset) : Instance (asset)
{
	lightNumber = count;
	count++;

	if(count >= 10)
		throw std::runtime_error("Too many lights created");
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

float Light::getAngle() {
	return angle;
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

void Light::setAngle(float ang) {
	angle = ang;
}

void Light::updateLighting(Program* program) {
	
	// Set Light
	GLint lightPos = program->getUniformStructLocation("allLights.position", lightNumber);
	GLint lightColor = program->getUniformStructLocation("allLights.intensities", lightNumber);
	GLint lightAtten = program->getUniformStructLocation("allLights.attenuation", lightNumber);
	GLint lightAmbient = program->getUniformStructLocation("allLights.ambientCoefficient", lightNumber);
	GLint lightAngle = program->getUniformStructLocation("allLights.coneAngle", lightNumber);
	GLint lightDriection = program->getUniformStructLocation("allLights.coneDirection", lightNumber);

	glUniform3fv(lightPos, 1, glm::value_ptr(getPosition()));
	glUniform3fv(lightColor, 1, glm::value_ptr(intensities));
	glUniform1f(lightAtten, attenuation);
	glUniform1f(lightAmbient, ambientCoefficient);
	glUniform1f(lightAngle, angle);
	glUniform3fv(lightDriection, 1, glm::value_ptr(getDirection()));
}
