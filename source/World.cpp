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
	Texture* crate = new Texture("wooden-crate", "jpg", 9729, 33071);
	Texture* normalMap = new Texture("Normal_Map", "bmp", 9729, 33071);
	Texture* bodyTex = new Texture("body2", "png", 9729, 33071);
	Texture* tireTex = new Texture("tire", "png", 9729, 33071);


	Asset* crateAsset = new Asset(Cube, crate, programs->at(TEX));	
	crateAsset->shininess = 80.0f;
	crateAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(CRATE, crateAsset));

	Asset* body = new Asset("micro_subaru_body", bodyTex, NULL, programs->at(TEX));
	body->shininess = 80.0f;
	body->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(BODY, body));

	Asset* wheel = new Asset("WheelCenter", NULL, tireTex, programs->at(NO_TEX_BUMP));
	wheel->shininess = 80.0f;
	wheel->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(WHEEL, wheel));

	Asset* interior = new Asset("micro_subaru_interior", NULL, NULL, programs->at(NO_TEX));
	interior->shininess = 80.0f;
	interior->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(INTERIOR, interior));

	Asset* boxAsset = new Asset(Cube, crate, programs->at(TEX));	
	boxAsset->shininess = 80.0f;
	boxAsset->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	assets.insert(std::pair<ASSETS, Asset*>(BOX, boxAsset));
}

void World::initLights() {
	Light* light = new Light(assets.at(BOX));
	light->translate(vec3(2, 0, 4));
	light->setIntensities(vec3(0.9, 0.9, 0.9));
	light->setAttenuation(0.01f);
	light->setAmbientCoefficient(0.05f);
	light->scale(vec3(0.1, 0.1, 0.1));
	light->setAngle(20.0f);

	lights.push_back(light);
		
	/*Light* light2 = new Light(assets.at(BOX));
	light2->translate(vec3(2, 0, -4));
	light2->setIntensities(vec3(0.9, 0.9, 0.9));
	light2->setAttenuation(0.01f);
	light2->setAmbientCoefficient(0.01f);
	light2->scale(vec3(0.1, 0.1, 0.1));
	light2->rotate(vec3(0, 1, 0), 180);
	light2->setAngle(35.0f);

	lights.push_back(light2);*/
}
	
void World::initInstances() {
	Instance* one = new Instance(assets.at(BOX));
	Instance* two = new Instance(assets.at(BOX));

	Instance* body = new Instance(assets.at(BODY));

	Instance* wh1 = new Instance(assets.at(WHEEL));
	Instance* wh2 = new Instance(assets.at(WHEEL));
	Instance* wh3 = new Instance(assets.at(WHEEL));
	Instance* wh4 = new Instance(assets.at(WHEEL));

	Instance* interior = new Instance(assets.at(INTERIOR));


	one->translate(vec3(-4, 0, 0));
	two->scale(vec3(1, 2, 1));

	one->shouldRotate = false;
	two->shouldRotate = false;

	instances.push_back(one);
	instances.push_back(two);


	body->translate(vec3(0, 0, 10));
	interior->translate(vec3(0, 0, 10));

	wh1->translate(vec3(-1.75, 0.75, 12.80));
	wh2->translate(vec3(1.75, 0.75, 12.80));
	wh3->translate(vec3(-1.75, 0.75, 7.20));
	wh4->translate(vec3(1.75, 0.75, 7.20));

	wh1->rotate(vec3(0, 0, 1), 180);
	wh3->rotate(vec3(0, 0, 1), 180);

	//instances.push_back(one);
	instances.push_back(interior);

	interior->shouldRotate = false;

	instances.push_back(wh1);
	instances.push_back(wh2);
	instances.push_back(wh3);
	instances.push_back(wh4);

	wh1->shouldRotate = true;
	wh2->shouldRotate = true;
	wh3->shouldRotate = true;
	wh4->shouldRotate = true;


	body->shouldRotate = false;
	instances.push_back(body);
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