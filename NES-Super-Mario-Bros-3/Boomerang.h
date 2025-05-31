#pragma once
#include "HarmfulObject.h"

#define BOOMERANG_FREQUENCY (M_PI / 1200.0f)
#define BOOMERANG_INITIAL_PHASE_VX 0.0f
#define BOOMERANG_INITIAL_PHASE_VY (M_PI / 3.0f)

#define BOOMERANG_VX_AMPLITUDE (4.0f / 75.0f * M_PI)
#define BOOMERANG_VY_AMPLITUDE (-1.0f / 60.0f * M_PI)

#define BOOMERANG_BBOX_WIDTH 16.0f
#define BOOMERANG_BBOX_HEIGHT 16.0f

#define ANI_ID_BOOMERANG 28000
#define ANI_ID_BOOMERANG_HOLD 0
#define ANI_ID_BOOMERANG_FLY 10

class CBoomerang :
    public CHarmfulObject
{
    float vx_oscillate_phase;
    float vy_oscillate_phase;

    int action;
public:
    CBoomerang(float x, float y);

    void Prepare(DWORD dt);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    int IsCollidable();

    void OnReactionTo(LPCOLLISIONEVENT e, int action);

    void Refresh();
    void Fire();
};

