#pragma once

#include "GameObject.h"
#include "PlayScene.h"

#define GREEN_KOOPA_GRAVITY 0.002f
#define GREEN_KOOPA_WALKING_SPEED 0.05f
#define GREEN_KOOPA_SHELL_MOVING_SPEED 0.2f

#define GREEN_KOOPA_BBOX_WIDTH 14
#define GREEN_KOOPA_BBOX_HEIGHT 18
#define GREEN_KOOPA_BBOX_HEIGHT_DIE 14
#define GREEN_KOOPA_BBOX_HEIGHT_WINGED 24
#define GREEN_KOOPA_SHELL_BBOX_WIDTH 16
#define GREEN_KOOPA_SHELL_BBOX_HEIGHT 14

#define GREEN_KOOPA_DIE_TIMEOUT 500
#define GREEN_KOOPA_SHELL_TIMEOUT 6000

#define GREEN_KOOPA_TYPE_WINGED 0
#define GREEN_KOOPA_TYPE_NORMAL 1

#define GREEN_KOOPA_STATE_WALKING 100
#define GREEN_KOOPA_STATE_JUMPING 200
#define GREEN_KOOPA_STATE_FALLING 300
#define GREEN_KOOPA_STATE_SHELL 400
#define GREEN_KOOPA_STATE_DIE 500
#define GREEN_KOOPA_STATE_SHELL_MOVING 600
#define GREEN_KOOPA_STATE_SHELL_HOLD 700

#define ID_ANI_GREEN_KOOPA_WINGED_WALKING 6200
#define ID_ANI_GREEN_KOOPA_WINGED_JUMPING 6201
#define ID_ANI_GREEN_KOOPA_WINGED_FALLING 6202
#define ID_ANI_GREEN_KOOPA_WINGED_DIE 6203

#define ID_ANI_GREEN_KOOPA_WALKING 6300
#define ID_ANI_GREEN_KOOPA_SHELL 6301
#define ID_ANI_GREEN_KOOPA_DIE 6302

#define ID_ANI_GREEN_KOOPA_SHELL_MOVING 6303

#define GREEN_KOOPA_JUMP_TIMEOUT 300
#define GREEN_KOOPA_JUMP_SPEED 0.15f

class CGreenKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG jump_start;
	ULONGLONG die_start;
	ULONGLONG shell_start;

	int type;
	bool isOnPlatform;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithRedKoopa(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e);

public:
	CGreenKoopa(float x, float y, int type);
	virtual void SetState(int state);

	int LeftOrRightMarrio();
};