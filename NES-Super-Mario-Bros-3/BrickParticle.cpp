#include "BrickParticle.h"

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

void CBrickParticle::Refresh()
{
	if (vy)
	{
		this->Delete();
		return;
	}

	CMovableObject::Refresh();

	vy = -5.0f;
}
