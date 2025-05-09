#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Background.h"
#include "Portal.h"
#include "DeadStateTrigger.h"
#include "Coin.h"
#include "Platform.h"
#include "VenusFireTrap.h"
#include "KoopaTroopa.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "QuestionBlock.h"
#include "Pipe.h"
#include "Container.h"
#include "Bush.h"
#include "StripedBrick.h"
#include "Cloud.h"
#include "MapIcon.h"
#include "BrickParticle.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	//key_handler = new CSampleKeyHandler(this);
	background = NULL;	
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define OFFSET 32

float tempCamPosY = 0;

bool isStartSpawned = false;

int coin = 0;

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

void CPlayScene::_ParseSection_ASSETS(string line)
{
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
	case NON_OBJECT_TYPE_BACKGROUND:
	{
		obj = new CBackground(x, y);
		background = (CBackground*)obj;
		return;
	}
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
		int length = atoi(tokens[5].c_str());
		int sprite_begin_begin = atoi(tokens[6].c_str());
		int sprite_middle_begin = atoi(tokens[7].c_str());
		int sprite_end_begin = atoi(tokens[8].c_str());
		int sprite_begin_end = atoi(tokens[9].c_str());
		int sprite_middle_end = atoi(tokens[10].c_str());
		int sprite_end_end = atoi(tokens[11].c_str());

		obj = new CCloud
		(x, y, cell_width, cell_height, length + 2,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_end, sprite_middle_end, sprite_end_end);
		break;
	}
	case NON_OBJECT_TYPE_MAP_ICON:
	{
		int icon_type = atoi(tokens[3].c_str());

		obj = new CMapIcon(x, y, icon_type);
		break;
	}

	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		if (isStartSpawned)
		{
			CGame::GetInstance()->GetNewPlayerPos(x, y);
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		isStartSpawned = true;
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_PARAGOOMBA: obj = new CParagoomba(x,y); break;
	case OBJECT_TYPE_VENUS_FIRE_TRAP: obj = new CVenusFireTrap(x, y); break;
	case OBJECT_TYPE_PIRANHA_PLANT: obj = new CPiranhaPlant(x, y); break;
	case OBJECT_TYPE_RED_KOOPA_TROOPA: obj = new CKoopaTroopa(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_STRIPED_BRICK: obj = new CStripedBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_SUPER_MUSHROOM: obj = new CSuperMushroom(x, y); break;
	case OBJECT_TYPE_SUPER_LEAF: obj = new CSuperLeaf(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		int sprite_begin = atoi(tokens[7].c_str());
		int sprite_middle = atoi(tokens[8].c_str());
		int sprite_end = atoi(tokens[9].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length, type,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
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

		obj = new CPipe(
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
		int length = atoi(tokens[5].c_str());
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
			length, height,
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
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	vector<LPGAMEOBJECT> nearbyObjects = FilterByPlayer();

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
			else if (obj->IsDirectionalBlocking() == false && obj->IsBlocking()) //For overlap
				staticColliders.push_back(obj);
		}
	}	

	//solve collision with blocking objects first
	for (auto& obj : movingColliders) //For moving objects
		CCollision::GetInstance()->SolveCollisionWithBlocking(obj, dt, &blockingColliders);

	for (auto& obj : staticColliders) //For static objects
		CCollision::GetInstance()->SolveOverlap(obj, &movingColliders);

	//solve collision with non-blocking objects
	for (auto& obj : movingColliders)
		CCollision::GetInstance()->SolveCollisionWithNonBlocking(obj, dt, &nonBlockingColliders);

	for (auto& obj : nearbyObjects)
		obj->Update(dt, &nearbyObjects);

	UpdateCamera();	

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	//if (background) background->Render();

	vector<LPGAMEOBJECT> process_list = FilterByCam();

	for (int i = 0; i < process_list.size(); i++)
	{
		process_list[i]->Render();
	}
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
		delete objects[i];

	objects.clear();
	player = NULL;

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

bool CPlayScene::IsInRange(LPGAMEOBJECT obj, float start_x, float end_x, float start_y, float end_y)
{
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	bool horizontally_inside = (left <= end_x) && (right >= start_x);
	bool vertically_inside = (top <= end_y) && (bottom >= start_y);

	return horizontally_inside && vertically_inside;

	//return !(left > max_x || right < min_x || top > max_y || bottom < min_y);
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
		if (IsInRange(objects[i], start_x, end_x, start_y, end_y))
			process_list.push_back(objects[i]);
	}

	return process_list;
}

vector<LPGAMEOBJECT> CPlayScene::FilterByCam()
{
	CGame* game = CGame::GetInstance();
	
	float start_x, end_x, start_y, end_y;
	game->GetCamPos(start_x, start_y);
	end_x = start_x + game->GetBackBufferWidth();
	end_y = start_y + game->GetBackBufferHeight();

	/*end_x = start_x + 256.0f;
	end_y = start_y + 192.0f;*/

	vector<LPGAMEOBJECT> process_list;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (IsInRange(objects[i], start_x, end_x, start_y, end_y))
			process_list.push_back(objects[i]);
	}

	return process_list;
}

void CPlayScene::UpdateCamera()
{
	// Update camera to follow mario
	CGame* game = CGame::GetInstance();

	float cx, cy;
	player->GetPosition(cx, cy);

	float player_bbox_height = player->GetBBoxHeight();

	cx = cx - game->GetBackBufferWidth() / 2.0f;
	cx = fmax(0.0f, cx);

	if (player->IsFlying())
	{
		cy = cy - game->GetBackBufferHeight() / 2.0f;
		cy = fmin(CAM_MAX_Y, cy);
	}
	else
	{
		cy = CAM_MAX_Y;
		//cy = cy + player_bbox_height / 2.0f + 16.0f - game->GetBackBufferHeight();
	}

	/*if (GetAsyncKeyState(VK_UP) & 0x8000) cy -= 10;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) cy += 10;*/

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
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