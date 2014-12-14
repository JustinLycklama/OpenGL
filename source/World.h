#pragma once

#include "Program.h"
#include "Texture.h"
#include "Asset.h"
#include "Camera.h"
#include "Instance.h"
#include "Light.h"

#include <map>

enum ASSETS {
	BOX, CRATE, MESH
};

class World
{
  public:
	World(map<PROGRAM_TYPE, Program*>* progs, Camera* camera);
	~World(void);

	vector<Light*>* getLights();
	vector<Instance*>* getInstances();
	vector<Instance*> getLightsAndInstances();

  private:
	void initAssets();
	void initLights();
	void initInstances();

	vector<Instance*> instances;
	vector<Texture*> textures;
	vector<Light*> lights;

	Camera* camera;
	map<ASSETS, Asset*> assets;
	map<PROGRAM_TYPE, Program*>* programs;
};

