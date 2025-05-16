#include "SuperLeaf.h"
#include "Mario.h"
#include "PlayScene.h"

void CSuperLeaf::Render()
{
	int aniId;
	if (state == SUPER_LEAF_STATE_BASIC)
		if (vx < 0) {
			aniId = ID_ANI_SUPER_LEAF_LEFT;
		} else {
			aniId = ID_ANI_SUPER_LEAF_RIGHT;
		}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	// RenderBoundingBox();
}

void CSuperLeaf::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - SUPER_LEAF_BBOX_WIDTH / 2;
	t = y - SUPER_LEAF_BBOX_HEIGHT / 2;
	r = l + SUPER_LEAF_BBOX_WIDTH;
	b = t + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (!canCollect)
		vy += ay * dt;

	if (state == SUPER_LEAF_STATE_BASIC)
	{
		oscillateTime += dt;
		if (oscillateTime > SUPER_LEAF_OCSILLATE_TIMEOUT)
		{ // Switch direction every 400ms
			DebugOut(L">>>Switch");
			oscillateDir = -oscillateDir;
			oscillateTime = 0;
		}
		vx = SUPER_LEAF_OSCILLIATE_SPEED * oscillateDir;
	}

	// DebugOut(L"[VANTOC] %f\n", vy);
	if (vy > SUPER_LEAF_MAX_SPEED_FALL)
	{

		Delete();
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CSuperLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CSuperLeaf::SetState(int l)
{
	switch (l)
	{
	case SUPER_LEAF_SUMMON_STATE:
		vy = -OUT_BRICK;
		canCollect = false;
		break;

	case SUPER_LEAF_STATE_BASIC:
		canCollect = true;
		oscillateTime = 0;
		break;
	}
	CGameObject::SetState(l);
}