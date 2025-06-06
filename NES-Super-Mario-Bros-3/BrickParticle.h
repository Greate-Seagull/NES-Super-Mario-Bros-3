#pragma once

#include "MovableObject.h"
#include "Animation.h"
#include "Animations.h"

#define BRICK_PARTICLE_SIZE 8
#define ID_PARTICLE_ROTATION -70000

#define X_BLAST_SPEED 0.1f
#define Y_ACCELERATION_A 0.025f
#define Y_ACCELERATION_B 0.05f

class CBrickParticle : public CMovableObject
{
protected:
	int corner;
public:
	CBrickParticle(float x, float y, int corner) : CMovableObject(x, y)
	{
		this->corner = corner;

		this->vy = 0.0f;

		aniID = ID_PARTICLE_ROTATION;
		SetBoundingBox(BRICK_PARTICLE_SIZE, BRICK_PARTICLE_SIZE);
	}

	int IsCollidable() { return 0; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Refresh();
};