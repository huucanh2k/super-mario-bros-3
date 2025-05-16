#include "SuperLeaf.h"
#include "Mario.h"
#include "PlayScene.h"

void CSuperLeaf::Render()
{
	int aniId;
	if (vx > 0)
	{
		aniId = ID_ANI_LEAF_RIGHT;

	}
	else aniId = ID_ANI_LEAF_LEFT;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CSuperLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//vy += ay * dt;
	//vx += ax * dt;

	if (y < limitY) {

		y = limitY;
		vy = 0;
		SetState(LEAF_FALL_STATE);

	}
	else {
		if (x < limitL)
		{
			x = limitL;
			vx = -vx;
		}
		else if (x > limitR)
		{
			x = limitR;
			vx = -vx;
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CSuperLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CSuperLeaf::SetState(int l) {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (l) {

	case LEAF_SUMMON_STATE:
		vy = -OUT_BRICK;
		vx = 0;
		break;

	case LEAF_FALL_STATE:
		vy = 0.01f;
		vx = -LEAF_FALL_SPEED_X;
		break;

	}
	CGameObject::SetState(l);
}