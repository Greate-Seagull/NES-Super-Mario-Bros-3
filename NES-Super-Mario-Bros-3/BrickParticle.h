#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BRICK_PARTICLE_SIZE 8
#define ID_PARTICLE_ROTATION -70000

#define X_BLAST_SPEED 0.1f
#define Y_ACCELERATION_A 0.025f
#define Y_ACCELERATION_B 0.05f

class CBrickParticle : public CGameObject
{
protected:
	int corner;
public:
	CBrickParticle(float x, float y, int corner) : CGameObject(x, y)
	{
		this->corner = corner;
		vy = -5;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};