#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "DeadStateTrigger.h"
#include "Coin.h"
#include "FloatingPlatform.h"
#include "Paragoomba.h"
#include "VenusFireTrap.h"
#include "RedKoopaTroopa.h"
#include "BoomerangBrother.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Brick.h"
#include "BrickParticle.h"
#include "QuestionBlock.h"
#include "Pipe.h"
#include "Container.h"
#include "Bush.h"
#include "StripedBrick.h"
#include "Cloud.h"
#include "MapIcon.h"
#include "PButton.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	//key_handler = new CSampleKeyHandler(this);
	background = DEFAULT_BACKGROUND_COLOR;
	hud = NULL;
	timer = TIMER_VALUE;

	youGotACard = NULL;
	courseClear = NULL;
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_LEVEL		3
#define SCENE_SECTION_CAMERA	4
#define SCENE_SECTION_BACKGROUND	5

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define OFFSET 32

#define MAX_CAMERA_POSITION_1 2560
#define MAX_CAMERA_POSITION_3 1792
#define MAX_CAMERA_POSITION_4 256

#define SCENE_SWITCH_WAIT_TIME 6000

#define MARIO_LEFT_BOUNDARY 8

#define MARIO_FLYING_THRESHOLD 3

bool isAboveFlyingArea = false;

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_LEVEL(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	next_level_scene = atoi(tokens[0].c_str());
}

void CPlayScene::_ParseSection_CAMERA(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	min_cam_x = atof(tokens[0].c_str());
	min_cam_y = atof(tokens[1].c_str());
	max_cam_x = atof(tokens[2].c_str());
	max_cam_y = atof(tokens[3].c_str());
	cam_vx = atof(tokens[4].c_str());
	cam_vy = atof(tokens[5].c_str());
}

void CPlayScene::_ParseSection_BACKGROUND(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	int haveBackGround = atoi(tokens[0].c_str());

	if (haveBackGround)
		background = NORMAL_BACKGROUND_COLOR;
}

void CPlayScene::_ParseSection_ASSETS(string line)
{;
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case NON_OBJECT_TYPE_ENDING:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int sprite_begin = atoi(tokens[5].c_str());
		int sprite_end = atoi(tokens[6].c_str());

		obj = new CEndLevel(x, y, width, height, sprite_begin, sprite_end);
		break;
	}
	case NON_OBJECT_TYPE_CARD_RANDOM:
	{
		int sprite_begin_begin = atoi(tokens[3].c_str());
		int sprite_middle_begin = atoi(tokens[4].c_str());
		int sprite_end_begin = atoi(tokens[5].c_str());
		int sprite_begin_middle = atoi(tokens[6].c_str());
		int sprite_middle_middle = atoi(tokens[7].c_str());
		int sprite_end_middle = atoi(tokens[8].c_str());
		int sprite_begin_end = atoi(tokens[9].c_str());
		int sprite_middle_end = atoi(tokens[10].c_str());
		int sprite_end_end = atoi(tokens[11].c_str());

		obj = new CRandomCard(x, y,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end);
		break;
	}
	case NON_OBJECT_TYPE_REWARD:
		obj = new CReward(x, y);
		break;
	case DEAD_STATE_TRIGGER:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());

		obj = new CDeadStateTrigger(x, y, width, height);
		break;
	}
	case NON_OBJECT_TYPE_MINI_BUSH: obj = new CMiniBush(x, y); break;
	case NON_OBJECT_TYPE_BIG_BUSH:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int sprite_begin_begin = atoi(tokens[6].c_str());
		int sprite_end_begin = atoi(tokens[7].c_str());
		int sprite_begin_end = atoi(tokens[8].c_str());
		int sprite_end_end = atoi(tokens[9].c_str());

		obj = new CBigBush
			(x, y, cell_width, cell_height, height,
			sprite_begin_begin, sprite_end_begin,
			sprite_begin_end, sprite_end_end);
		break;
	}
	case NON_OBJECT_TYPE_CLOUD:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int width = atoi(tokens[5].c_str());
		int sprite_begin_begin = atoi(tokens[6].c_str());
		int sprite_middle_begin = atoi(tokens[7].c_str());
		int sprite_end_begin = atoi(tokens[8].c_str());

		if (tokens.size() > 9)
		{
			int sprite_begin_end = atoi(tokens[9].c_str());
			int sprite_middle_end = atoi(tokens[10].c_str());
			int sprite_end_end = atoi(tokens[11].c_str());

			obj = new CCloud
			(x, y, cell_width, cell_height, width + 2,
				sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
				sprite_begin_end, sprite_middle_end, sprite_end_end);
			break;
		}
		else
		{
			obj = new CCloud
			(x, y, cell_width, cell_height, width, 
				sprite_begin_begin, sprite_middle_begin, sprite_end_begin);
			break;
		}
	}
	case NON_OBJECT_TYPE_MAP_ICON:
	{
		int icon_type = atoi(tokens[3].c_str());

		obj = new CMapIcon(x, y, icon_type);
		break;
	}
	case OBJECT_TYPE_MARIO:
		if (player == NULL)
		{
			obj = new CMario(x,y);
			player = (CMario*)obj;
		}
		else
		{
			player->SetTraveled();
			if(player->IsInGround()) player->GetDestinationPosition(x, y);
			obj = player;
			DebugOut(L"[INFO] Player object has been created!\n");
		}
		break;
	case OBJECT_TYPE_GOOMBA: 
	{
		int haveWings = atoi(tokens[3].c_str());
		obj = new CGoomba(x,y,haveWings);
		spawner.Add(obj);
		break;
	}
	case OBJECT_TYPE_PARAGOOMBA: 
	{
		int haveWings = atoi(tokens[3].c_str());
		obj = new CParagoomba(x,y,haveWings); 
		spawner.Add(obj);
		break;
	}
	case OBJECT_TYPE_VENUS_FIRE_TRAP: 
		obj = new CVenusFireTrap(x, y); 
		break;
	case OBJECT_TYPE_PIRANHA_PLANT: 
		obj = new CPiranhaPlant(x, y); 
		break;
	case OBJECT_TYPE_KOOPA:
	{
		int haveWings = atoi(tokens[3].c_str());
		obj = new CKoopaTroopa(x, y, haveWings);
		spawner.Add(obj);
		break;
	}
	case OBJECT_TYPE_RED_KOOPA_TROOPA: 
	{
		int haveWings = atoi(tokens[3].c_str());
		obj = new CRedKoopaTroopa(x, y, haveWings);
		spawner.Add(obj);
		break;
	}
	case OBJECT_TYPE_BOOMERANG_BROTHER:
	{
		obj = new CBoomerangBrother(x, y);
		Insert(obj, -1);
		vector<CBoomerang*> boomerangs = ((CBoomerangBrother*)obj)->GetBoomerangs();
		for (int i = 0; i < boomerangs.size(); i++) Insert(boomerangs[i], -1);
		return;
	}
	case OBJECT_TYPE_BRICK: 
	{
		int itemID = atoi(tokens[3].c_str());
		int toggleCount = atoi(tokens[4].c_str());
		int transform = atoi(tokens[5].c_str());
		obj = new CBrick(x, y, itemID, toggleCount, transform);
		break;
	}
	case OBJECT_TYPE_STRIPED_BRICK: obj = new CStripedBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_SUPER_MUSHROOM: obj = new CSuperMushroom(x, y); break;
	case OBJECT_TYPE_SUPER_LEAF: obj = new CSuperLeaf(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int width = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());

		int sprite_begin_begin = atoi(tokens[8].c_str());
		int sprite_middle_begin = atoi(tokens[9].c_str());
		int sprite_end_begin = atoi(tokens[10].c_str());
		int sprite_begin_middle = atoi(tokens[11].c_str());
		int sprite_middle_middle = atoi(tokens[12].c_str());
		int sprite_end_middle = atoi(tokens[13].c_str());
		int sprite_begin_end = atoi(tokens[14].c_str());
		int sprite_middle_end = atoi(tokens[15].c_str());
		int sprite_end_end = atoi(tokens[16].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height,
			width, height,
			type,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end			
		);

		break;
	}
	case OBJECT_TYPE_FLOATING_PLATFORM:
	{
		float vx = (float)atof(tokens[3].c_str());
		float vy = (float)atof(tokens[4].c_str());
		float cell_width = (float)atof(tokens[5].c_str());
		float cell_height = (float)atof(tokens[6].c_str());
		int width = atoi(tokens[7].c_str());
		int height = atoi(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());

		int sprite_begin_begin = atoi(tokens[10].c_str());
		int sprite_middle_begin = atoi(tokens[11].c_str());
		int sprite_end_begin = atoi(tokens[12].c_str());
		int sprite_begin_middle = atoi(tokens[13].c_str());
		int sprite_middle_middle = atoi(tokens[14].c_str());
		int sprite_end_middle = atoi(tokens[15].c_str());
		int sprite_begin_end = atoi(tokens[16].c_str());
		int sprite_middle_end = atoi(tokens[17].c_str());
		int sprite_end_end = atoi(tokens[18].c_str());

		obj = new CFloatingPlatform(
			x, y, vx, vy,
			cell_width, cell_height,
			width, height,
			type,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end
		);

		break;
	}
	case OBJECT_TYPE_PBUTTON: obj = new CPButton(x, y); break;
	case OBJECT_TYPE_QUESTION_BLOCK: 
	{
		int itemID = atoi(tokens[3].c_str());
		obj = new CQuestionBlock(x, y, itemID);
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int pipe_height = atoi(tokens[5].c_str());

		int sprite_id_begin_begin = atoi(tokens[6].c_str());
		int sprite_id_end_begin = atoi(tokens[7].c_str());
		int sprite_id_begin_end = atoi(tokens[8].c_str());
		int sprite_id_end_end = atoi(tokens[9].c_str());

		int face_direction = atoi(tokens[10].c_str());
		int warp_direction = atoi(tokens[11].c_str());

		int itemID = atoi(tokens[12].c_str());
		if (warp_direction != 0)
		{
			int scene_destination = atoi(tokens[13].c_str());
			float newX = (float)atof(tokens[14].c_str());
			float newY = (float)atof(tokens[15].c_str());

			obj = new CPipe(
				x, y,
				cell_width, cell_height, pipe_height,
				face_direction, warp_direction,
				sprite_id_begin_begin, sprite_id_end_begin,
				sprite_id_begin_end, sprite_id_end_end,
				itemID, scene_destination, newX, newY);
		}
		else obj = new CPipe(
			x, y,
			cell_width, cell_height, pipe_height, 
			face_direction, warp_direction, 
			sprite_id_begin_begin, sprite_id_end_begin, 
			sprite_id_begin_end, sprite_id_end_end,
			itemID);
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());

		float new_x = (float)atof(tokens[6].c_str());
		float new_y = (float)atof(tokens[7].c_str());
		
		obj = new CPortal(x, y, r, b, scene_id, new_x, new_y);
	}
	break;
	case OBJECT_TYPE_CONTAINER:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int width = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		int sprite_begin_begin = atoi(tokens[7].c_str());
		int sprite_middle_begin = atoi(tokens[8].c_str());
		int sprite_end_begin = atoi(tokens[9].c_str());
		int sprite_begin_middle = atoi(tokens[10].c_str());
		int sprite_middle_middle = atoi(tokens[11].c_str());
		int sprite_end_middle = atoi(tokens[12].c_str());
		int sprite_begin_end = atoi(tokens[13].c_str());
		int sprite_middle_end = atoi(tokens[14].c_str());
		int sprite_end_end = atoi(tokens[15].c_str());

		obj = new CContainer(
			x, y,
			cell_width, cell_height,
			width, height,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end
		);

		break;
	}

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[LEVEL]") { section = SCENE_SECTION_LEVEL; continue; }
		if (line == "[CAMERA]") { section = SCENE_SECTION_CAMERA; continue; }
		if (line == "[BACKGROUND]") { section = SCENE_SECTION_BACKGROUND; continue; }
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_LEVEL: _ParseSection_LEVEL(line); break;
			case SCENE_SECTION_CAMERA: _ParseSection_CAMERA(line); break;
			case SCENE_SECTION_BACKGROUND: _ParseSection_BACKGROUND(line); break;
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	//HUD
	hud = new CHud();

	//CAMERA
	UpdateCamera(0);

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)

	if (isPaused)
		return;

	if (player->IsFastTravel())
	{
		FastTravel(dt);
		return;
	}

	if (player == NULL) 
		return;

	timer -= dt;		

	CCollision* collisionProcessor = CCollision::GetInstance();
	CCollisionTracker* collisionTracker = collisionProcessor->GetTracker();

	spawner.Spawn();

	vector<LPGAMEOBJECT> nearbyObjects = FilterByCam();

	for (auto& obj : nearbyObjects)
	{
		obj->Prepare(dt);
	}

	vector<LPGAMEOBJECT> movingColliders;
	vector<LPGAMEOBJECT> staticColliders;

	vector<LPGAMEOBJECT> blockingColliders;
	vector<LPGAMEOBJECT> nonBlockingColliders;
	for (auto& obj : nearbyObjects)
	{	
		if (obj->IsCollidable())
		{
			//Collide with objects
			if (obj->IsBlocking())
				blockingColliders.push_back(obj);
			else
				nonBlockingColliders.push_back(obj);

			//Objects will use collision process
			if (obj->IsMoving()) //For sweptAABB
				movingColliders.push_back(obj);
			else if (obj->IsDirectionalBlocking() == false) //For overlap
				staticColliders.push_back(obj);

			collisionTracker->Allocate(obj);
		}
	}

	//solve collision with blocking objects first
	for (auto& obj : movingColliders) //For moving objects
		collisionProcessor->SolveCollisionWithBlocking(obj, dt, &blockingColliders);

	for (auto& obj : staticColliders) //For static objects
		collisionProcessor->SolveOverlap(obj, dt, &movingColliders);

	//solve collision with non-blocking objects
	for (auto& obj : movingColliders)
		collisionProcessor->SolveCollisionWithNonBlocking(obj, dt, &nonBlockingColliders);

	for (auto& obj : nearbyObjects)
		obj->Update(dt, &nearbyObjects);

	UpdateCamera(dt);
	// UPDATE HUD AFTER MARIO UPDATED
	UpdateHUD();

	PurgeDeletedObjects();
}

//void CPlayScene::CollectingScore()
//{
//	timerPause = true;
//	if (timer > 0)
//	{
//		if (timer >= 10000)
//		{
//			score = score + 10 * SCORE_PER_SECOND;
//			timer -= 10000;
//		}
//		else if (timer >= 1000)
//		{
//			score = score + SCORE_PER_SECOND;
//			timer -= 1000;
//		}
//	}
//}

void CPlayScene::Render()
{
	//if (background) background->Render();

	vector<LPGAMEOBJECT> process_list = FilterByCam();

	for (int i = 0; i < process_list.size(); i++)
	{
		process_list[i]->Render();
	}

	hud->Render();

	RenderCongratulations();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();	
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		if(objects[i] != player)
			delete objects[i];

	objects.clear();
	player = NULL;

	spawner.Clear();

	delete hud;
	hud = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::Insert(LPGAMEOBJECT newObj, int index)
{
	if (!newObj)
		return;
	else if (index < 0)
		objects.push_back(newObj);
	else
		objects.insert(objects.begin() + index, newObj);

	spawner.Add(newObj);
}

int CPlayScene::Find(LPGAMEOBJECT obj)
{
	if (obj)
	{
		for (int i = 0; i < objects.size(); i++)
			if (objects.at(i) == obj)
				return i;
	}
	return -1;
}

vector<LPGAMEOBJECT> CPlayScene::FilterByPlayer(float range)
{
	float player_x, player_y;
	player->GetPosition(player_x, player_y);

	float range_width = range;
	float range_height = range;

	float start_x = player_x - range_width, end_x = player_x + range_width;
	float start_y = player_y - range_height, end_y = player_y + range_height;

	vector<LPGAMEOBJECT> process_list;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (CGame::IsInRange(objects[i], start_x, end_x, start_y, end_y))
			process_list.push_back(objects[i]);
	}

	return process_list;
}

vector<LPGAMEOBJECT> CPlayScene::FilterByCam()
{
	CGame* game = CGame::GetInstance();

	vector<LPGAMEOBJECT> process_list;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == player || game->IsInCam(objects[i]))
			process_list.push_back(objects[i]);
	}

	if (process_list.size() == 1)
		process_list.clear();

	return process_list;
}

void CPlayScene::UpdateCamera(DWORD dt)
{
	// Update camera to follow mario
	CGame* game = CGame::GetInstance();

	float cx, cy;
	game->GetCamPos(cx, cy);

	float px, py;
	player->GetPosition(px, py);

	//Move cam
	if (cam_vx)
		cx += dt * cam_vx;		
	else 
		cx = px - CAM_WIDTH / 2.0f;

	if (cam_vy)
		cy += dt * cam_vy;
	else
	{
		if (player->IsFlying()) cy = fmin(py - CAM_FOLLOW_HEIGHT, cy);	//Move cam to follow flying
		cy = fmax(py - CAM_HEIGHT / 2.0f, cy); //Move cam to follow falling
	}

	//Restrict cam
	cx = fmax(min_cam_x, cx);
	cx = fmin(max_cam_x, cx);

	cy = fmax(min_cam_y, cy);
	cy = fmin(max_cam_y, cy);

	game->SetCamPos(cx, cy);

	if(cam_vx)
	{
		px = max(px, cx + 16.0f);
		px = min(px, cx + CAM_WIDTH - 16.0f);
	}
	if (cam_vy)
	{
		py = max(py, cy + 16.0f);
		py = min(py, cy + CAM_HEIGHT - 16.0f);
	}

	player->SetPosition(px, py);
}
	
void CPlayScene::UpdateHUD()
{
	CGame* game = CGame::GetInstance();

	float cx, cy;
	game->GetCamPos(cx, cy);

	hud->SetPosition(cx + HUD_WIDTH / 2.0f, cy + CAM_HEIGHT + HUD_HEIGHT / 2.0f);

	hud->UpdateTime(timer);
	hud->UpdateCoin(player->GetCoins());
	hud->UpdateScore(player->GetScores());
	hud->UpdateMomentum(player->GetMomentum());
	hud->UpdateCards(player->GetCards());
}

void CPlayScene::RenderCongratulations()
{
	if (courseClear)
		courseClear->Render();
	if (youGotACard)
		youGotACard->Render();
	if (card)
		card->Render();
}

void CPlayScene::Congratulations()
{
	if (wait_time > COURSE_CLEAR_TIME)
	{
		if (wait_time >= SCORE_COLLECTING_TIME)
		{
			//CollectingScore();
		}
		else if (wait_time >= YOU_GOT_A_CARD_TIME)
		{
			if (youGotACard == NULL)
			{
				float cx, cy;
				CGame::GetInstance()->GetCamPos(cx, cy);

				youGotACard = new CClearText(cx + CONGRATULATIONS_ROW_CAMOFFSET - DIGIT_NEAR_SPACING * 2.0f, cy + CONGRATULATIONS_LINE2_CAMOFFSET, YOU_GOT_A_CARD_TEXT);
				card = new CHUDCard(player->GetLatestCard());
				card->SetPosition(youGotACard->GetX() + DIGIT_NEAR_SPACING * 10.0f, youGotACard->GetY() + DIGIT_NEAR_SPACING);
			}
		}
		else
		{
			if (courseClear == NULL)
			{
				float cx, cy;
				CGame::GetInstance()->GetCamPos(cx, cy);

				courseClear = new CClearText(cx + CONGRATULATIONS_ROW_CAMOFFSET, cy + CONGRATULATIONS_LINE1_CAMOFFSET, COURSE_CLEAR_TEXT);
			}
		}
	}
}

void CPlayScene::SwitchScene(int next_level)
{
	if (next_level_scene != id)	CGame::GetInstance()->InitiateSwitchScene(next_level);

	if (next_level_scene == 3)
	{
		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx, cy);
		CGame::GetInstance()->SetCamPos(0, cy);
	}
}

void CPlayScene::FastTravel(DWORD dt)
{
	if (player->IsInGround())
	{
		SwitchScene(player->GetDestination());
	}
	else
	{
		wait_time += dt;
		if (wait_time >= SCENE_SWITCH_WAIT_TIME)
		{
			SwitchScene(next_level_scene);
		}
		else
		{
			Congratulations();
		}
	}
}

//void CPlayScene::InsertScore(int score_value)
//{
//	score += score_value;
//}

//void CPlayScene::InsertScore(float x, float y, int score_value)
//{
//	CScore* scoreUI = new CScore(x, y, score_value);
//	int playerIndex = Find(player);
//	Insert(scoreUI, playerIndex - 1);
//
//	if (score_value == 10000) InsertScore(0);
//	else InsertScore(score_value);
//}

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			spawner.Delete(o);			
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}