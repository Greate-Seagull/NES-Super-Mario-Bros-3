#pragma once

#include "MovableObject.h"
#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define BRICK_PARTICLE_SIZE 8
#define ID_PARTICLE_ROTATION -70000

#define X_BLAST_SPEED 0.1f
#define Y_BLAST_SPEED -5.0f
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

		switch (this->corner)
		{
		case 1: SetSpeed(-X_BLAST_SPEED, Y_BLAST_SPEED); break;
		case 2: SetSpeed(X_BLAST_SPEED, Y_BLAST_SPEED); break;
		case 3: SetSpeed(-X_BLAST_SPEED, Y_BLAST_SPEED); break;
		case 4: SetSpeed(X_BLAST_SPEED, Y_BLAST_SPEED); break;
		}

		aniID = ID_PARTICLE_ROTATION;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};