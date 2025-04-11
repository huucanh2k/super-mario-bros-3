#pragma once

#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 14
#define KOOPA_BBOX_HEIGHT_DIE 7

#define KOOPA_DIE_TIMEOUT 500
#define KOOPA_SHELL_TIMEOUT 50000

#define KOOPA_SHELL_SPEED 0

#define KOOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_SHELL_BBOX_HEIGHT 14

#define KOOPA_STATE_WALKING_LEFT 100
#define KOOPA_STATE_WALKING_RIGHT 101
#define KOOPA_STATE_SHELL 200
#define KOOPA_STATE_DIE 300

#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_SHELL 6002
#define ID_ANI_KOOPA_DIE 6003

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	ULONGLONG die_start;
	ULONGLONG shell_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};