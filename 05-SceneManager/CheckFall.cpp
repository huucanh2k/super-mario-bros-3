#include "CheckFall.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Game.h"
#include "Goomba.h"
#include "Box.h"

#define OBJ_BBOX_WIDTH 13
#define OBJ_BBOX_HEIGHT 18

CCheckFall::CCheckFall(float x, float y, int model) :CGameObject(x, y)
{

	this->model = model;
	this->ax = 0;
	this->ay = 0.016f;
	//isOnPlatformCheck = false;
	//OnTheBox = true;
	//SetState(STATE_LEFT_KOOPA);
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
	//if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) isOnPlatformCheck = true;
	}


	//if (dynamic_cast<CCheckFall*>(e->obj)) return;

	//if (e->nx != 0)
		//isDeleted = true;

	//if (dynamic_cast<CBackground*>(e->obj))
		//this->OnCollisionWithPlatForm(e);
	if (dynamic_cast<CBox*>(e->obj))
		this->OnCollisionWithBox(e);
	/*else if (dynamic_cast<CglassBrick*>(e->obj))
		OnCollisionWithGlassBrick(e)*/;

}

//void CCheckFall::OnCollisionWithGlassBrick(LPCOLLISIONEVENT e) {
//
//
//
//	CglassBrick* glassBrick = dynamic_cast<CglassBrick*>(e->obj);
//	if (e->ny != 0)
//	{
//		vy = 0;
//		if (e->ny < 0) isOnPlatformCheck = true;
//
//		model = 2;
//	}
//
//
//}

void CCheckFall::OnCollisionWithPlatForm(LPCOLLISIONEVENT e) {
	CBackground* platform = dynamic_cast<CBackground*>(e->obj);

	isOnPlatformCheck = true;
	vy = 0;

}
void CCheckFall::OnCollisionWithBox(LPCOLLISIONEVENT e) {
	CBox* OntheBox = dynamic_cast<CBox*>(e->obj);

	//if (GetX() < (x-10)) isDeleted = true;

	//if (GetX() > OntheBox->endBox) isDeleted = true;
}

void CCheckFall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	isOnPlatformCheck = false;


	//if (GetX() < (x - 10)) isDeleted = true;
	/*if (vx < 0) {
		SetState(STATE_LEFT_KOOPA);
	}
	else if(vx>0) SetState(STATE_RIGHT_KOOPA);*/

	//if (isOnPlatformCheck) isDeleted = true;


	//if (!OnTheBox) isDeleted = true;

	//if (!OnTheBox) isDeleted = true;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

//void CCheckFall::OnCollisionWithPlatForm(LPCOLLISIONEVENT e)
//{
//
//}


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