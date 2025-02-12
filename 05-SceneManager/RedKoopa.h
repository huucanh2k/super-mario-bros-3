#pragma once

#include "GameObject.h"
#include "CheckFall.h"
 
#define ID_ANI_RED_KOOPA_WALKING_LEFT 6100
#define ID_ANI_RED_KOOPA_WALKING_RIGHT 6101
#define ID_ANI_RED_KOOPA_SHELL 6102
#define ID_ANI_RED_KOOPA_DIE 6103
#define ID_ANI_RED_KOOPA_SHELL_FAST_MOVING 6104
#define ID_ANI_RED_KOOPA_SHELL_HOLD 6105

class CRedKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG shell_start;

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

public:
	CRedKoopa(float x, float y);

	virtual void AddCheck(CGameObject* obj);
	virtual void SetState(int state);

	int LeftOrRightMarrio();
};