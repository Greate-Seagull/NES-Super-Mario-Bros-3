#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"

#include "SpawnManager.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Paragoomba.h"
#include "HUD.h"

#define CAM_MAX_Y 235.0f
#define COLLISION_RANGE 300.0f

#define CAM_SPEED 0.03f
#define TIMER_VALUE 300000

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;
	LPGAMEOBJECT background;

	float timer; //scene time
	bool isPaused;
	CHud* hud;	

	vector<LPGAMEOBJECT> objects;
	CSpawnManager spawner;

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

	int GetCurrentSceneID() { return id; }

	LPGAMEOBJECT GetPlayer() { return player; }
	
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void Insert(LPGAMEOBJECT newObj, int index);
	int Find(LPGAMEOBJECT obj);
	vector<LPGAMEOBJECT> FilterByPlayer(float range = COLLISION_RANGE);
	vector<LPGAMEOBJECT> FilterByCam();
	void UpdateCamera(DWORD dt);
	void UpdateHUD();

};

typedef CPlayScene* LPPLAYSCENE;

