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
		Accelerate(0, Y_ACCELERATION_A, dt);
		break;
	}
	case 2: {
		Accelerate(0, Y_ACCELERATION_A, dt);
		break;
	}
	case 3: {
		Accelerate(0, Y_ACCELERATION_B, dt);
		break;
	}
	case 4: {
		Accelerate(0, Y_ACCELERATION_B, dt);
		break;
	}
	}

	x += vx * dt;
	y += vy;
}