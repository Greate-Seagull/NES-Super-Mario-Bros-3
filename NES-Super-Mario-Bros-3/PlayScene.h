#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Paragoomba.h"
#include "HUD.h"
//#include "Koopas.h"

#define CAM_MAX_Y 283.0f
#define COLLISION_RANGE 300.0f

#define CAM_SPEED 0.03f

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;
	LPGAMEOBJECT background;
	CHud* hud;
	CDigit* scoreDigits[DIGIT_COUNT_SCORE];
	CDigit* coinDigits[DIGIT_COUNT_CURRENCY];
	CDigit* timeDigits[DIGIT_COUNT_TIME];
	CPMeter* pMeter[P_METER_COUNT];

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

	int GetCurrentSceneID() { return id; }

	LPGAMEOBJECT GetPlayer() { return player; }
	
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void Insert(LPGAMEOBJECT newObj, int index);
	int Find(LPGAMEOBJECT obj);
	bool IsInRange(LPGAMEOBJECT obj, float start_x, float end_x, float start_y, float end_y);
	vector<LPGAMEOBJECT> FilterByPlayer(float range = COLLISION_RANGE);
	vector<LPGAMEOBJECT> FilterByCam();
	void UpdateCamera(DWORD dt);

	void UpdateTime();
	void CollectCoin();
	void UpdateCoin();
	void UpdateScore();
	void UpdatePMeter();
	void UpdateRunTime(DWORD dt, bool isProgress);
};

typedef CPlayScene* LPPLAYSCENE;

