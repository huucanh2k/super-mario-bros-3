#include "RedParaGoomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "Animation.h"
#include "debug.h"
#include "Platform.h"
#include "RedKoopa.h"

CRedParaGoomba::CRedParaGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = RED_PARA_GOOMBA_GRAVITY;
	die_start = -1;
	jumpping_start = -1;
	walking_start = -1;
	isWinged = true;
	isJumping = false;
	hopCount = 0;
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
	else if (isWinged) {
		left = x - RED_PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_WINGED_PARA_GOOMBA_HEIHT / 2;
		right = left + RED_PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_WINGED_PARA_GOOMBA_HEIHT;
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
		if (dynamic_cast<CRedKoopa*>(e->obj)) {
			OnCollisionWithRedKoopa(e);
		}
	}

	if (dynamic_cast<CRedParaGoomba*>(e->obj) ) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
}

void CRedParaGoomba::OnCollisionWithRedKoopa(LPCOLLISIONEVENT e) {
	if (e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT) {
		if (isWinged) {
			isWinged = false;
		}
		else {
			SetState(RED_PARA_GOOMBA_STATE_DIE);
		}
		return;
	}
}

void CRedParaGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e) {
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (e->ny < 0) {
		isOnPlatform = true;
		y = platform->GetY() - RED_PARA_GOOMBA_BBOX_HEIGHT + 1;
	}
}

void CRedParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CGameObject::IsInScreen())
	{
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if ((state == RED_PARA_GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > RED_PARA_GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (isWinged && isOnPlatform) {
		if (state == RED_PARA_GOOMBA_STATE_WALKING && GetTickCount64() - walking_start > RED_PARA_GOOMBA_WALKING_TIMEOUT) {
			if (hopCount < 2) {
				SetState(RED_PARA_GOOMBA_STATE_HOPPING);
				hopCount++;
			}
			else {
				SetState(RED_PARA_GOOMBA_STATE_JUMPING);
				hopCount = 0;
			}
		}
		else if ((state == RED_PARA_GOOMBA_STATE_JUMPING || state == RED_PARA_GOOMBA_STATE_HOPPING) && GetTickCount64() - jumpping_start > RED_PARA_GOOMBA_JUMPING_TIMEOUT) {
			SetState(RED_PARA_GOOMBA_STATE_WALKING);
			DebugOut(L">>>Check walking");
		}
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
		else if (state == RED_PARA_GOOMBA_STATE_HOPPING)
			aniId = ID_ANI_RED_WINGED_PARA_GOOMBA_HOPPING;
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
		walking_start = GetTickCount64();
		vy = 0;
		break;

	case RED_PARA_GOOMBA_STATE_JUMPING:
		if (mario->GetX() > x)
			vx = RED_PARA_GOOMBA_WALKING_SPEED;
		else
			vx = -RED_PARA_GOOMBA_WALKING_SPEED;
		vy = -RED_PARA_GOOMBA_FLYING_SPEED;
		isOnPlatform = false;
		jumpping_start = GetTickCount64();
		break;
	case RED_PARA_GOOMBA_STATE_HOPPING:
		if (mario->GetX() > x)
			vx = RED_PARA_GOOMBA_WALKING_SPEED;
		else
			vx = -RED_PARA_GOOMBA_WALKING_SPEED;
		vy = -RED_PARA_GOOMBA_HOPPING_SPEED;
		isOnPlatform = false;
		break;
	}
}
