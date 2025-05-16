#include "MushRoom.h"

#include "Platform.h"
#include "PlayScene.h"
#include "Mario.h"

CMushRoom::CMushRoom(float x, float y, int model_mushroom) :CGameObject(x, y)
{
	this->model_mushroom = model_mushroom;
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	vy = 0;
	startY = y;
	SetState(MUSHROOM_STATE_OUTSIDE);
}

void CMushRoom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking() && !e->obj->IsPlatform()) return;
	if (!e->obj->IsPlayer()) {
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}

	if (dynamic_cast<CMushRoom*>(e->obj)) {}
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatForm(e);
	else if (dynamic_cast<CMario*>(e->obj))
	{
		Delete();
	}

}

void CMushRoom::OnCollisionWithPlatForm(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (platform->IsBlocking()) {}
	else if (e->ny < 0) {
		SetY(platform->GetY() - MUSHROOM_BBOX_HEIGHT);
	}
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (state == MUSHROOM_STATE_EXISTS) {
		vy += ay * dt;
		vx += ax * dt;
	}

	else if (state == MUSHROOM_STATE_OUTSIDE) {
		if (startY - y < MUSHROOM_BBOX_HEIGHT) {
			vy = OUT_BRICK;
			vx = 0;
		}
		else SetState(MUSHROOM_STATE_EXISTS);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushRoom::Render()
{
	int aniId = -1;

	if (model_mushroom == MODE_RED) aniId = ID_ANI_MUSHROOM;

	else if (model_mushroom == MODE_GREEN) aniId = ID_ANI_MUSHROOM_GREEN;


	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (state)
	{
	case MUSHROOM_STATE_EXISTS:
		if (x < mario->GetX()) vx = -MUSHROOM_SPEED;
		else vx = MUSHROOM_SPEED;
		break;
	}
	CGameObject::SetState(state);
}