#include "World.h"

World::World(map<PROGRAM_TYPE, Program*>* progs, Camera* cam)
{
	programs = progs;
	camera = cam;

	initAssets();
	initLights();
	initInstances();
}


World::~World(void)
{
	for(vector<Instance*>::iterator it = instances.begin(); it != instances.end(); ++it){ free(*it); }
	for(vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it){ free(*it); }
	for(map<ASSETS, Asset*>::iterator it = assets.begin(); it != assets.end(); ++it){ free(it->second); }
	for(vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it){ free(*it); }
}

void World::initAssets() {
	// Load Textures
	Texture* crate = new Texture("wooden-crate.jpg", 9729, 33071);

	Asset* crateAsset = new Asset(Cube, crate, programs->at(TEX));	
	crateAsset->shininess = 80.0f;
	crateAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(CRATE, crateAsset));

	Asset* mesh = new Asset("micro_subaru_body", NULL, programs->at(NO_TEX));
	mesh->shininess = 80.0f;
	mesh->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(MESH, mesh));

	Asset* boxAsset = new Asset(Cube, NULL, programs->at(NO_TEX));	
	boxAsset->shininess = 80.0f;
	boxAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(BOX, boxAsset));
}

void World::initLights() {
	Light* light = new Light(assets.at(BOX));
	light->translate(vec3(2, 0, 4));
	light->setIntensities(vec3(0.9, 0.9, 0.9));
	light->setAttenuation(0.01f);
	light->setAmbientCoefficient(0.01f);
	light->scale(vec3(0.1, 0.1, 0.1));
	light->setAngle(10.0f);

	lights.push_back(light);
		
	Light* light2 = new Light(assets.at(BOX));
	light2->translate(vec3(2, 0, -4));
	light2->setIntensities(vec3(0.9, 0.9, 0.9));
	light2->setAttenuation(0.01f);
	light2->setAmbientCoefficient(0.01f);
	light2->scale(vec3(0.1, 0.1, 0.1));
	light2->rotate(vec3(0, 1, 0), 180);
	light2->setAngle(35.0f);

	lights.push_back(light2);
}
	
void World::initInstances() {
	Instance* one = new Instance(assets.at(CRATE));
	Instance* two = new Instance(assets.at(BOX));
	Instance* three = new Instance(assets.at(MESH));

	two->translate(vec3(4, 0, 0));
	three->translate(vec3(-4, 0, 0));

	instances.push_back(one);
	instances.push_back(two);
	instances.push_back(three);
}

vector<Light*>* World::getLights() {
	return &lights;
}

vector<Instance*>* World::getInstances() {
	return &instances;
}

vector<Instance*> World::getLightsAndInstances() {
	vector<Instance*> objects = instances;
	objects.insert(objects.end(), lights.begin(), lights.end());

	return objects;
}