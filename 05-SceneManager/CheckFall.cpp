#include "CheckFall.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Game.h"
#include "Goomba.h"
#include "Box.h"
#include "debug.h"

#define OBJ_BBOX_WIDTH 1
#define OBJ_BBOX_HEIGHT 12

CCheckFall::CCheckFall(float x, float y, int model) :CGameObject(x, y)
{
	this->model = model;
	this->ax = 0;
	this->ay = 0.016f;
}

void CCheckFall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (model == CHECKFALL_NORMAL) {
		l = x - OBJ_BBOX_WIDTH / 2;
		t = y - OBJ_BBOX_HEIGHT / 2;
		r = l + OBJ_BBOX_WIDTH;
		b = t + OBJ_BBOX_HEIGHT;
	}
	else if (model == CHECKFALL_SMALL)
	{
		l = x - BOX_SMALL_BBOX_WIDTH / 2;
		t = y - BOX_SMALL_BBOX_HEIGHT / 2;
		r = l + BOX_SMALL_BBOX_WIDTH;
		b = t + BOX_SMALL_BBOX_HEIGHT;
	}
}

void CCheckFall::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CCheckFall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) isOnPlatformCheck = true;
	}

	if (dynamic_cast<CBox*>(e->obj))
		this->OnCollisionWithBox(e);
}

void CCheckFall::OnCollisionWithBox(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;

	CBox* box = dynamic_cast<CBox*>(e->obj);
}

void CCheckFall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	isOnPlatformCheck = false;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CCheckFall::Render()
{

	RenderBoundingBox();
}

void CCheckFall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		if (model == 1) {
	case STATE_LEFT_KOOPA:
		vx = -SPEED_PREVIOUS_KOOPA;
		vy = 0;

		break;

	case STATE_RIGHT_KOOPA:
		vx = SPEED_PREVIOUS_KOOPA;
		vy = 0;
		break;
		}

		else if (model == 2)
		{
	case SMALL_STATE_LEFT_KOOPA:

		vx = -SMALL_SPEED_PREVIOUS_KOOPA;
		ay = 0.012f;
		vy = 0;

		break;

	case SMALL_STATE_RIGHT_KOOPA:


		vx = SMALL_SPEED_PREVIOUS_KOOPA;
		vy = 0;
		ay = 0.012f;
		break;
		}
	}
}