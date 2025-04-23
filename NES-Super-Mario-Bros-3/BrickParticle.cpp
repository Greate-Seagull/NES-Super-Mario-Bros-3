#include "BrickParticle.h"

void CBrickParticle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

void CBrickParticle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (this->corner)
	{
	case 1: {
		vx = -X_BLAST_SPEED * dt;
		vy += Y_ACCELERATION_A * dt;
		break;
	}
	case 2: {
		vx = X_BLAST_SPEED * dt;
		vy += Y_ACCELERATION_A * dt;
		break;
	}
	case 3: {
		vx = -X_BLAST_SPEED * dt;
		vy += Y_ACCELERATION_B * dt;
		break;
	}
	case 4: {
		vx = X_BLAST_SPEED * dt;
		vy += Y_ACCELERATION_B * dt;
		break;
	}
	}
	
	x += vx;
	y += vy;
}