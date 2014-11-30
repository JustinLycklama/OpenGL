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

vec3 Light::getPosition() {
	return position;
}

float Light::getAttenuation() {
	return attenuation;
}

float Light::getAmbientCoefficient() {
	return ambientCoefficient;
}

void Light::setPosition(vec3 pos) {
	position = pos;
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

