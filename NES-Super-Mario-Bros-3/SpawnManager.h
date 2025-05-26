#pragma once

#include "GameObject.h"

#define DEFAULT_X -999.0f
#define DEFAULT_Y -999.0f

using namespace std;

class CSpawnManager
{
	unordered_map<CGameObject*, pair<float, float>> objectPositions;

public:
	void Add(CGameObject* obj);
	void Spawn();
	void Delete(CGameObject* obj);	
};

