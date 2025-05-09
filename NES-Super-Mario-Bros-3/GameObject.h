#pragma once

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include <string>
#include <cmath>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

#define STATE_DIE -1
#define STATE_LIVE 0

#define GAME_GRAVITY			0.001f

class CGameObject
{
protected:

	float x; 
	float y;

	float bbox_height;
	float bbox_width;

	int state;

	//animation
	int aniID;

	bool isDeleted; 

	void SetBoundingBox(float width, float height);

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	float GetX() { return x; }
	float GetY() { return y; }

	virtual void GetSpeed(float &vx, float &vy) { vx = 0.0f; vy = 0.0f; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);	
	virtual float GetBBoxHeight() { return bbox_height; }
	virtual float GetBBoxWidth() { return bbox_width; }

	virtual void Prepare(DWORD dt) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();
	virtual void SetState(int state);
	virtual void ClearState() { state = -100; }
	
	//When an object is updated, collision scanning will not detect it
	virtual int IsUpdated() { return 0; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	//Active
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	//Inactive
	virtual void Reaction(CGameObject* by_another, int action) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }

	virtual const char* ToString();
};
