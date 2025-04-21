#include "RedParaGoomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "Animation.h"
#include "debug.h"

CRedParaGoomba::CRedParaGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = RED_PARA_GOOMBA_GRAVITY;
	die_start = -1;
	jump_count_start = -1;
	isWinged = true;
	isJumping = false;
	SetState(RED_PARA_GOOMBA_STATE_WALKING);
}

void CRedParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == RED_PARA_GOOMBA_STATE_DIE)
	{
		left = x - RED_PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_PARA_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_PARA_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - RED_PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_PARA_GOOMBA_BBOX_HEIGHT / 2;
		right = left + RED_PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_PARA_GOOMBA_BBOX_HEIGHT;
	}
}

void CRedParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) {
		if (dynamic_cast<CRedParaGoomba*>(e->obj)) {
			if (e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT) {
				SetState(RED_PARA_GOOMBA_STATE_DIE);
				return;
			}
		}
		return;
	}

	if (dynamic_cast<CRedParaGoomba*>(e->obj) ) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CRedParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == RED_PARA_GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > RED_PARA_GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (state == RED_PARA_GOOMBA_STATE_WALKING && GetTickCount64() - jump_count_start > RED_PARA_GOOMBA_WALKING_TIMEOUT)
	{
		SetState(RED_PARA_GOOMBA_STATE_JUMPING);
	}
	else if (state == RED_PARA_GOOMBA_STATE_JUMPING && isOnPlatform)
	{
		SetState(RED_PARA_GOOMBA_STATE_WALKING);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CRedParaGoomba::Render()
{
	int aniId = ID_ANI_RED_WINGED_PARA_GOOMBA_WALKING;

	if (isWinged) {
		if (state == RED_PARA_GOOMBA_STATE_WALKING)
			aniId = ID_ANI_RED_WINGED_PARA_GOOMBA_WALKING;
		else if (state == RED_PARA_GOOMBA_STATE_JUMPING)
			aniId = ID_ANI_RED_WINGED_PARA_GOOMBA_JUMPING;
	}
	else {
		if (state == RED_PARA_GOOMBA_STATE_WALKING)
			aniId = ID_ANI_RED_PARA_GOOMBA_WALKING;
	}

	if (state == RED_PARA_GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_RED_PARA_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CRedParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (state)
	{
	case RED_PARA_GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (RED_PARA_GOOMBA_BBOX_HEIGHT - RED_PARA_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case RED_PARA_GOOMBA_STATE_WALKING:
		if (mario->GetX() > x )
			vx = RED_PARA_GOOMBA_WALKING_SPEED;
		else 
			vx = -RED_PARA_GOOMBA_WALKING_SPEED;
		vy = 0;
		isOnPlatform = true;
		vy = 0;
		jump_count_start = GetTickCount64();
		break;

	case RED_PARA_GOOMBA_STATE_JUMPING:
		if (mario->GetX() > x)
			vx = RED_PARA_GOOMBA_WALKING_SPEED;
		else
			vx = -RED_PARA_GOOMBA_WALKING_SPEED;
		vy = -RED_PARA_GOOMBA_FLYING_SPEED;
		isOnPlatform = false;
		break;

	case RED_PARA_GOOMBA_STATE_FALLING:
		vx = 0;
		vy = RED_PARA_GOOMBA_GRAVITY;
		isJumping = false;
		break;
	}
}
