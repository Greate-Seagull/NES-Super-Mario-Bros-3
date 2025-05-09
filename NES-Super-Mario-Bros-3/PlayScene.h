#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Paragoomba.h"
//#include "Koopas.h"


#define CAM_MAX_Y 247.0f
#define COLLISION_RANGE 300.0f

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;
	LPGAMEOBJECT background;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void Insert(LPGAMEOBJECT newObj, int index);
	int Find(LPGAMEOBJECT obj);
	bool IsInRange(LPGAMEOBJECT obj, float start_x, float end_x, float start_y, float end_y);
	vector<LPGAMEOBJECT> FilterByPlayer(float range = COLLISION_RANGE);
	vector<LPGAMEOBJECT> FilterByCam();
	void UpdateCamera();
};

typedef CPlayScene* LPPLAYSCENE;

