#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height; 

	float new_x, new_y;
public:
	CPortal(float l, float t, float r, float b, int scene_id, float new_x, float new_y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }
	int IsBlocking() { return 0; }

	void GetNewPlayerPos(float& x, float& y)
	{
		x = this->new_x;
		y = this->new_y;
	}
};