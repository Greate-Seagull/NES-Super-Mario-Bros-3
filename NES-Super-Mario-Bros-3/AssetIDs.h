#pragma once

//
// This file contains all IDs of game assets 
//

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20


#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_COIN 4
#define OBJECT_TYPE_PLATFORM 5
#define OBJECT_TYPE_VENUS_FIRE_TRAP 6
#define OBJECT_TYPE_RED_KOOPA_TROOPA 7
#define OBJECT_TYPE_PIRANHA_PLANT 8
#define OBJECT_TYPE_PARAGOOMBA 9
#define OBJECT_TYPE_SUPER_MUSHROOM 10
#define OBJECT_TYPE_SUPER_LEAF 11

#define OBJECT_TYPE_PORTAL	50


#pragma region MARIO 
//ID FOR OBJECT
#define ID_SPRITE_MARIO 10000
#define ID_SPRITE_ENEMY 20000
//#define ID_SPRITE_MARIO_BIG (ID_SPRITE_MARIO + 1000)
//#define ID_SPRITE_MARIO_BIG_IDLE (ID_SPRITE_MARIO_BIG + 100)
//#define ID_SPRITE_MARIO_BIG_IDLE_LEFT (ID_SPRITE_MARIO_BIG_IDLE + 10)
//#define ID_SPRITE_MARIO_BIG_IDLE_RIGHT (ID_SPRITE_MARIO_BIG_IDLE + 20)
//
//#define ID_SPRITE_MARIO_BIG_WALKING (ID_SPRITE_MARIO_BIG + 200)
//#define ID_SPRITE_MARIO_BIG_WALKING_LEFT (ID_SPRITE_MARIO_BIG_WALKING + 10)
//#define ID_SPRITE_MARIO_BIG_WALKING_RIGHT (ID_SPRITE_MARIO_BIG_WALKING + 20)
//#define ID_SPRITE_MARIO_BIG_RUNNING (ID_SPRITE_MARIO_BIG + 300)
//#define ID_SPRITE_MARIO_BIG_RUNNING_LEFT (ID_SPRITE_MARIO_BIG_RUNNING + 10)
//#define ID_SPRITE_MARIO_BIG_RUNNING_RIGHT (ID_SPRITE_MARIO_BIG_RUNNING + 20)
//#define ID_SPRITE_MARIO_BIG_JUMP (ID_SPRITE_MARIO_BIG + 400)
//#define ID_SPRITE_MARIO_BIG_JUMP_WALK (ID_SPRITE_MARIO_BIG_JUMP + 10)
//#define ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 2)
//#define ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 6)
//#define ID_SPRITE_MARIO_BIG_JUMP_RUN (ID_SPRITE_MARIO_BIG_JUMP + 20)
//#define ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 2)
//#define ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 6)
//#define ID_SPRITE_MARIO_BIG_SIT (ID_SPRITE_MARIO_BIG + 500)
//#define ID_SPRITE_MARIO_BIG_SIT_LEFT (ID_SPRITE_MARIO_BIG_SIT + 10)
//#define ID_SPRITE_MARIO_BIG_SIT_RIGHT (ID_SPRITE_MARIO_BIG_SIT + 20)
//#define ID_SPRITE_MARIO_BIG_BRACE (ID_SPRITE_MARIO_BIG + 600)
//#define ID_SPRITE_MARIO_BIG_BRACE_LEFT (ID_SPRITE_MARIO_BIG_BRACE + 10)
//#define ID_SPRITE_MARIO_BIG_BRACE_RIGHT (ID_SPRITE_MARIO_BIG_BRACE + 20)

//#define ID_SPRITE_MARIO_DIE (ID_SPRITE_MARIO + 3000)
//-------------------------------------------------SMALL--------------------------------------------------
#define ID_SPRITE_MARIO_SMALL (ID_SPRITE_MARIO + 0)
//Idle
#define ID_SPRITE_MARIO_SMALL_IDLE (ID_SPRITE_MARIO_SMALL + 10)
#define ID_SPRITE_MARIO_SMALL_IDLE_LEFT (ID_SPRITE_MARIO_SMALL_IDLE + 1)
#define ID_SPRITE_MARIO_SMALL_IDLE_RIGHT (ID_SPRITE_MARIO_SMALL_IDLE + 2)
//Walking
#define ID_SPRITE_MARIO_SMALL_WALKING (ID_SPRITE_MARIO_SMALL + 20)
#define ID_SPRITE_MARIO_SMALL_WALKING_LEFT (ID_SPRITE_MARIO_SMALL_WALKING + 1)
#define ID_SPRITE_MARIO_SMALL_WALKING_RIGHT (ID_SPRITE_MARIO_SMALL_WALKING + 2)
//Jumping
#define ID_SPRITE_MARIO_SMALL_JUMP (ID_SPRITE_MARIO_SMALL + 30)
#define ID_SPRITE_MARIO_SMALL_JUMP_LEFT (ID_SPRITE_MARIO_SMALL_JUMP + 1)
#define ID_SPRITE_MARIO_SMALL_JUMP_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP + 2)
//-------------------------------------------------SMALL--------------------------------------------------
#define ID_SPRITE_MARIO_BIG (ID_SPRITE_MARIO + 1000)
//Idle
#define ID_SPRITE_MARIO_BIG_IDLE (ID_SPRITE_MARIO_BIG + 10)
#define ID_SPRITE_MARIO_BIG_IDLE_LEFT (ID_SPRITE_MARIO_BIG_IDLE + 1)
#define ID_SPRITE_MARIO_BIG_IDLE_RIGHT (ID_SPRITE_MARIO_BIG_IDLE + 2)
//Walking
#define ID_SPRITE_MARIO_BIG_WALKING (ID_SPRITE_MARIO_BIG + 20)
#define ID_SPRITE_MARIO_BIG_WALKING_LEFT (ID_SPRITE_MARIO_BIG_WALKING + 1)
#define ID_SPRITE_MARIO_BIG_WALKING_RIGHT (ID_SPRITE_MARIO_BIG_WALKING + 2)
//Running
#define ID_SPRITE_MARIO_BIG_RUN (ID_SPRITE_MARIO_BIG + 30)
#define ID_SPRITE_MARIO_BIG_RUN_LEFT (ID_SPRITE_MARIO_BIG_RUN + 1)
#define ID_SPRITE_MARIO_BIG_RUN_RIGHT (ID_SPRITE_MARIO_BIG_RUN + 2)
//Jumping
#define ID_SPRITE_MARIO_BIG_JUMP (ID_SPRITE_MARIO_BIG + 40)
#define ID_SPRITE_MARIO_BIG_JUMP_LEFT (ID_SPRITE_MARIO_BIG_JUMP + 1)
#define ID_SPRITE_MARIO_BIG_JUMP_RIGHT (ID_SPRITE_MARIO_BIG_JUMP + 2)
//Jumping
#define ID_SPRITE_MARIO_BIG_SIT (ID_SPRITE_MARIO_BIG + 50)
#define ID_SPRITE_MARIO_BIG_SIT_LEFT (ID_SPRITE_MARIO_BIG_SIT + 1)
#define ID_SPRITE_MARIO_BIG_SIT_RIGHT (ID_SPRITE_MARIO_BIG_SIT + 2)
//#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 6)
//#define ID_SPRITE_MARIO_SMALL_JUMP_RUN (ID_SPRITE_MARIO_SMALL_JUMP + 20)
//#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 2)
//#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 6)

//#define ID_SPRITE_MARIO_SMALL_SIT (ID_SPRITE_MARIO_SMALL + 500)
//#define ID_SPRITE_MARIO_SMALL_SIT_LEFT (ID_SPRITE_MARIO_SMALL_SIT + 10)
//#define ID_SPRITE_MARIO_SMALL_SIT_RIGHT (ID_SPRITE_MARIO_SMALL_SIT + 20)
//
//#define ID_SPRITE_MARIO_SMALL_BRACE (ID_SPRITE_MARIO_SMALL + 500)
//#define ID_SPRITE_MARIO_SMALL_BRACE_LEFT (ID_SPRITE_MARIO_SMALL_BRACE + 10)
//#define ID_SPRITE_MARIO_SMALL_BRACE_RIGHT (ID_SPRITE_MARIO_SMALL_BRACE + 20)

#pragma endregion 

#define ID_SPRITE_BRICK 30000

#pragma region ENEMY
//-------------------------------------------------GOOMBA--------------------------------------------------
#define ID_SPRITE_GOOMBA (ID_SPRITE_ENEMY + 0)
//Walking
#define ID_SPRITE_GOOMBA_WALK (ID_SPRITE_GOOMBA + 0)
#define ID_SPRITE_GOOMBA_WALK_LEFT_FOOT (ID_SPRITE_GOOMBA_WALK + 1)
#define ID_SPRITE_GOOMBA_WALK_RIGHT_FOOT (ID_SPRITE_GOOMBA_WALK + 2)
//Dying
#define ID_SPRITE_GOOMBA_DIE (ID_SPRITE_GOOMBA + 10)
//-------------------------------------------------VENUS FIRE TRAP--------------------------------------------------
#define ID_SPRITE_VENUS (ID_SPRITE_ENEMY + 1000)
#define ID_SPRITE_VENUS_
#pragma endregion 

#define ID_SPRITE_COIN 40000

#define ID_SPRITE_CLOUD 50000
#define ID_SPRITE_CLOUD_BEGIN (ID_SPRITE_CLOUD+1000)
#define ID_SPRITE_CLOUD_MIDDLE (ID_SPRITE_CLOUD+2000)
#define ID_SPRITE_CLOUD_END (ID_SPRITE_CLOUD+3000)