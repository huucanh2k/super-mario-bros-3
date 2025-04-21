#pragma once
#include "GameObject.h"

#define RED_PARA_GOOMBA_GRAVITY 0.002f
#define RED_PARA_GOOMBA_WALKING_SPEED 0.025f
#define RED_PARA_GOOMBA_FLYING_SPEED 0.1f
#define RED_PARA_GOOMBA_WALKING	0.05f


#define RED_PARA_GOOMBA_BBOX_WIDTH 16
#define RED_PARA_GOOMBA_BBOX_HEIGHT 14
#define RED_PARA_GOOMBA_BBOX_HEIGHT_DIE 10

#define RED_PARA_GOOMBA_DIE_TIMEOUT 500
#define RED_PARA_GOOMBA_WALKING_TIMEOUT 1000

#define RED_PARA_GOOMBA_STATE_WALKING 100
#define RED_PARA_GOOMBA_STATE_JUMPING 200
#define RED_PARA_GOOMBA_STATE_DIE 300
#define RED_PARA_GOOMBA_STATE_BE_WHIPED	400
#define RED_PARA_GOOMBA_STATE_DIE 500
#define RED_PARA_GOOMBA_STATE_FALLING 600
#define RED_PARA_GOOMBA_STATE_BE_WHIPED 700


#define ID_ANI_RED_WINGED_PARA_GOOMBA_WALKING 5500
#define ID_ANI_RED_WINGED_PARA_GOOMBA_JUMPING 5501
#define ID_ANI_RED_PARA_GOOMBA_WALKING 5502
#define ID_ANI_RED_PARA_GOOMBA_DIE 5503

class CRedParaGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG jump_count_start;
	bool isWinged;
	bool isJumping;
	bool isOnPlatform;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedParaGoomba(float x, float y);
	virtual void SetState(int state);
};