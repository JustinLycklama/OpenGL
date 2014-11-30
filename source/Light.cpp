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

void Light::setPosition(vec3 pos) {
	position = pos;
}

void Light::setIntensities(vec3 col) {
	intensities = col;
}

