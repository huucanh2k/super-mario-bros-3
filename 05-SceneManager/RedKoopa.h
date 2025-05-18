#pragma once

#include "GameObject.h"
#include "CheckFall.h"

#define RED_KOOPA_STATE_WALKING_LEFT 100
#define RED_KOOPA_STATE_WALKING_RIGHT 101
#define RED_KOOPA_STATE_SHELL 200
#define RED_KOOPA_STATE_DIE 300
#define RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT 400
#define RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT 401
#define RED_KOOPA_STATE_SHELL_HOLD 500
#define RED_KOOPA_STATE_FLYING 600
 
#define ID_ANI_RED_KOOPA_WALKING_LEFT 6100
#define ID_ANI_RED_KOOPA_WALKING_RIGHT 6101
#define ID_ANI_RED_KOOPA_SHELL 6102
#define ID_ANI_RED_KOOPA_DIE 6103
#define ID_ANI_RED_KOOPA_SHELL_FAST_MOVING 6104
#define ID_ANI_RED_KOOPA_SHELL_HOLD 6105

#define ID_ANI_RED_KOOPA_WINGED_FLYING 6150

#define RED_KOOPA_TYPE_WINGED 0
#define RED_KOOPA_TYPE_NORMAL 1

#define RED_KOOPA_GRAVITY 0.002f
#define RED_KOOPA_WALKING_SPEED 0.05f
#define RED_KOOPA_SHELL_MOVING_SPEED 0.2f
#define RED_KOOPA_WINGED_FLYING_SPEED 0.1f

#define RED_KOOPA_WINGED_BBOX_WIDTH 14
#define RED_KOOPA_WINGED_BBOX_HEIGHT 24
#define RED_KOOPA_BBOX_WIDTH 14
#define RED_KOOPA_BBOX_HEIGHT 18
#define RED_KOOPA_BBOX_HEIGHT_DIE 14
#define RED_KOOPA_SHELL_BBOX_WIDTH 16
#define RED_KOOPA_SHELL_BBOX_HEIGHT 14

#define RED_KOOPA_DIE_TIMEOUT 500
#define RED_KOOPA_SHELL_TIMEOUT 6000

#define RED_KOOPA_MAX_FLY_HEIGHT 100
#define RED_KOOPA_MIN_FLY_HEIGHT 0
#define RED_KOOPA_FLY_SPEED 0.05f

class CRedKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG shell_start;
	int type;

	CCheckFall* checkFall;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void CreateCheckFall();

	virtual void CheckFall();
	virtual void OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e);	

public:
	CRedKoopa(float x, float y, int type);

	virtual void AddCheck(CGameObject* obj);
	virtual void SetState(int state);

	int LeftOrRightMarrio();
};