#pragma once

#include <Windows.h>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent 
{
	LPGAMEOBJECT src_obj;		// source object : the object from which to calculate collision
	LPGAMEOBJECT obj;			// the target object
	
	float t, nx, ny;

	float dx, dy;				// *RELATIVE* movement distance between this object and obj
	bool isDeleted;		

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, 
		LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
		this->isDeleted = false;
	}

	int WasCollided();

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CCollisionEventManager
{
	vector<unique_ptr<CCollisionEvent>> co_events;
	int current_event;

public:
	CCollisionEventManager();

	void Add(float t, float nx, float ny, float dx, float dy, LPGAMEOBJECT objDest, LPGAMEOBJECT objSrc);
	void VirtualDelete();
	void Refresh();
	LPCOLLISIONEVENT Get(int i);
	LPCOLLISIONEVENT GetLast();
	int Size();
};

class CCollision
{
	static CCollision* __instance;

	CCollisionEventManager e_manager;
public: 
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	/*LPCOLLISIONEVENT SweptAABB(
		LPGAMEOBJECT objSrc, 
		DWORD dt,
		LPGAMEOBJECT objDest); 
	void Scan(
		LPGAMEOBJECT objSrc, 
		DWORD dt, 
		vector<LPGAMEOBJECT>* objDests, 
		vector<LPCOLLISIONEVENT>& coEvents);

	void Filter(
		LPGAMEOBJECT objSrc,
		vector<LPCOLLISIONEVENT>& coEvents,
		LPCOLLISIONEVENT &colX,
		LPCOLLISIONEVENT &colY, 
		int filterBlock,		
		int filterX,
		int filterY);*/

	void SweptAABB(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		LPGAMEOBJECT objDest);
	void Scan(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		vector<LPGAMEOBJECT>* objDests);

	void Filter(
		LPGAMEOBJECT objSrc,		
		LPCOLLISIONEVENT& colX,
		LPCOLLISIONEVENT& colY,
		int filterBlock,
		int filterX,
		int filterY);

	void Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	bool Overlap(LPGAMEOBJECT objSrc, LPGAMEOBJECT objDst);
	void ProcessOverlap(LPGAMEOBJECT objSrc, vector<LPGAMEOBJECT>* coObjects);

	static CCollision* GetInstance();
};