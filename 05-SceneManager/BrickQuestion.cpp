#include "BrickQuestion.h"
#include "Mario.h"



CBrickQuestion::CBrickQuestion(float x, float y, int model) :CGameObject(x, y)
{
	this->model = model;
	this->ay = 0;
	this->minY = y - BRICK_Q_BBOX_HEIGHT / 2;
	this->startY = y;
	this->startX = x;
	isEmpty = false;
	isUnbox = false;
}



void CBrickQuestion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isUnbox) {
		if (model == BRICK_QUESTION_COIN) {
			l = (x - BRICK_Q_BBOX_WIDTH / 2) - 4;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}
		else if (model == BRICK_QUESTION_NOT_COIN) {
			l = (x - BRICK_Q_BBOX_WIDTH / 2) - 4;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}

		else if (model == BRICK_QUESTION_LEAF)
		{
			l = x - BRICK_Q_BBOX_WIDTH / 2;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}

	}
	else {
		if (model == BRICK_QUESTION_COIN)
		{
			l = x - BRICK_Q_BBOX_WIDTH / 2;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH - 6;
			b = t + BRICK_Q_BBOX_HEIGHT;

		}
		else if (model == BRICK_QUESTION_LEAF) {
			l = x - BRICK_Q_BBOX_WIDTH / 2;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}
		else if (model == BRICK_QUESTION_NOT_COIN)
		{
			l = x - BRICK_Q_BBOX_WIDTH / 2;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}
		else
		{
			l = x - BRICK_Q_BBOX_WIDTH / 2;
			t = y - BRICK_Q_BBOX_HEIGHT / 2;
			r = l + BRICK_Q_BBOX_WIDTH;
			b = t + BRICK_Q_BBOX_HEIGHT;
		}
	}
}

void CBrickQuestion::OnNoCollision(DWORD dt)
{
	//x += vx * dt;
	//y += vy * dt;
	//x = 0; Chi xet brick question di chuyen theo phuong y
	y += vy * dt;

};



void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	vy += ay * dt;


	if (y <= minY)
	{
		vy = BRICK_Q_SPEED_DOWN;
	}
	if (y > startY + BRICK_Q_BBOX_HEIGHT - ADJUST_UP_DOWN)
	{
		y = startY;
		vy = 0;
	}


	if (isUnbox) isEmpty = true;



	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}




void CBrickQuestion::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CBrickQuestion*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = 0;
	}
}


void CBrickQuestion::Render()
{

	int aniId;
	if (model == BRICK_QUESTION_COIN || BRICK_QUESTION_MUSHROOM || BRICK_QUESTION_LEAF || BRICK_QUESTION_NOT_COIN) {
		aniId = ID_ANI_BRICK_QUESTION;
	}
	if (isEmpty || isUnbox)
	{
		aniId = ID_ANI_BRICK_EMPTY;
	}
	/*if (state == BrickQuestion_STATE_EMPTY)
	{
		aniId = ID_ANI_BRICK_EMPTY;
	}*/

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();


}

void CBrickQuestion::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_QUESTION_STATE_EMPTY:
		vy = 0;
		isEmpty = true;
		isUnbox = true;
		break;

	case BRICK_QUESTION_STATE_UP:
		vy = -BRICK_Q_SPEED_UP;
		break;
	}

}