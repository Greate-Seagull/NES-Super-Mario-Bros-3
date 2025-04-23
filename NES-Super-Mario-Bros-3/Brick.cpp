#include "Brick.h"

CBrick::CBrick(float x, float y):
	CQuestionBlock(x, y)
{
	bbox_width = BRICK_BBOX_WIDTH;
	bbox_height = BRICK_BBOX_HEIGHT;

	aniID = ID_ANI_BRICK;
}