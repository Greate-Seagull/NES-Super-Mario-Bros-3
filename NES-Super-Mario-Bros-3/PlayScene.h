#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"

#include "SpawnManager.h"
#include "Background.h"
#include "Mario.h"
#include "HUD.h"

#define CAM_MAX_Y 231.0f
#define COLLISION_RANGE 300.0f

#define CAM_SPEED 0.03f
#define MAX_CAMERA_POSITION 2000.0f
#define TIMER_VALUE 300000

#define CONGRATULATIONS_ROW_CAMOFFSET CAM_WIDTH / 2.0f
#define CONGRATULATIONS_LINE1_CAMOFFSET 16.0f * 3
#define CONGRATULATIONS_LINE2_CAMOFFSET 16.0f * 4

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

	int wait_time = 0;
	int next_level_scene;

	//Congratulations
	CClearText* courseClear;
	CClearText* youGotACard;
	CHUDCard* card;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_LEVEL(string line);
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

	void SetPlayer(LPGAMEOBJECT player) { this->player = dynamic_cast<CMario*>(player); }
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

	void RenderCongratulations();

	vector<LPGAMEOBJECT> GetObjects() { return objects; }

	void Congratulations();
	void SwitchScene(int next_level);

	void FastTravel(DWORD dt);
	/*void InsertScore(int score_value);
	void InsertScore(float x, float y, int score_value);*/
};

typedef CPlayScene* LPPLAYSCENE;

