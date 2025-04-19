
#include "RedKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "Box.h"
#include "Mario.h"
 
CRedKoopa::CRedKoopa(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	shell_start = -1;
	checkFall = NULL;
	SetState(KOOPA_STATE_WALKING_LEFT);
	CreateCheckFall();
}


void CRedKoopa::Render()
{
	int ani = ID_ANI_RED_KOOPA_WALKING_LEFT;
	if (state == KOOPA_STATE_WALKING_RIGHT)
		ani = ID_ANI_RED_KOOPA_WALKING_RIGHT;
	else if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_HOLD)
		ani = ID_ANI_RED_KOOPA_SHELL;
	else if (state == KOOPA_STATE_DIE)
		ani = ID_ANI_RED_KOOPA_DIE;
	else if (state == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || state == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
		ani = ID_ANI_KOOPA_SHELL_FAST_MOVING;

	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CRedKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_DIE)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else if (state == KOOPA_STATE_SHELL)
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else if (state == KOOPA_STATE_SHELL_HOLD) {
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CRedKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedKoopa*>(e->obj)) {
		if (e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || e->obj->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT) {
			SetState(KOOPA_STATE_DIE);
			return;
		}
	};

	if (e->ny != 0)
	{
		vy = 0;
	}

	else if (e->nx != 0)
	{
		if (dynamic_cast<CBox*>(e->obj)) {
			if (state == KOOPA_STATE_WALKING_LEFT) {
				SetState(KOOPA_STATE_WALKING_RIGHT);
			}
			else if (state == KOOPA_STATE_WALKING_RIGHT) {
				SetState(KOOPA_STATE_WALKING_LEFT);
			}
			else if (state == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
			{
				SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
			}
			else if (state == KOOPA_STATE_SHELL_FAST_MOVING_LEFT) {
				SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
			}
		}
	}
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
vy += ay * dt;
vx += ax * dt;
CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
{
	isDeleted = true;
	return;
}
if ((state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_HOLD) && (GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT))
{
	SetState(KOOPA_STATE_WALKING_LEFT);
	return;
}

if (mario->GetIsHolding()) {
	if (state == KOOPA_STATE_SHELL)
	{
		SetState(KOOPA_STATE_SHELL_HOLD);
	}
	else if (state == KOOPA_STATE_SHELL_HOLD) {
		float marioX, marioY, marioDirection;
		marioX = mario->GetX();
		marioY = mario->GetY();
		SetPosition(marioX + mario->GetNx() * MARIO_BIG_BBOX_WIDTH - 1, marioY - 1);
	}
	return;
}
else {
	if (state == KOOPA_STATE_SHELL_HOLD)
	{
		if (LeftOrRightMarrio() == 1)
		{
			SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
			DebugOut(L">>> Check turn into shell moving right >>> \n");
		}
		else if (LeftOrRightMarrio() == -1)
		{
			SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
			DebugOut(L">>> Check turn into shell moving left >>> \n");
		}
	}
}

if (checkFall != NULL && (GetState() != KOOPA_STATE_SHELL_FAST_MOVING_LEFT && GetState() != KOOPA_STATE_SHELL_FAST_MOVING_RIGHT))
{
	CheckFall();
}

CGameObject::Update(dt, coObjects);
CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedKoopa::AddCheck(CGameObject* obj) {
	if (!dynamic_cast<CCheckFall*>(obj)) return;
	else if (!checkFall)
	{
		CCheckFall* cfall_obj = dynamic_cast<CCheckFall*>(obj);
		checkFall = cfall_obj;
	
		DebugOut(L">>> check >>> \n");

	}
}

int CRedKoopa::LeftOrRightMarrio() {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() < GetX()) return 1;
	else return -1;
}


void CRedKoopa::CheckFall()
{
	float offsetX = (vx < 0) ? -KOOPA_BBOX_WIDTH : KOOPA_BBOX_WIDTH; // Offset based on direction
	checkFall->SetPosition(x + offsetX, y);

	// Check if checkFall is no longer on a platform
	if (checkFall->IsFalling())
	{
		// Turn Koopa before falling
		if (GetState() == KOOPA_STATE_WALKING_LEFT)
		{
			SetState(KOOPA_STATE_WALKING_RIGHT);
			checkFall->SetPosition(x + -offsetX, y);
			DebugOut(L">>> check turn right >>> \n");
		}
		else if (KOOPA_STATE_WALKING_RIGHT)
		{
			SetState(KOOPA_STATE_WALKING_LEFT);
			checkFall->SetPosition(x + -offsetX, y);
			DebugOut(L">>> check turn left >>> \n");
		}
	}
}

void CRedKoopa::CreateCheckFall()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	float offsetX = (vx < 0) ? -KOOPA_BBOX_WIDTH : KOOPA_BBOX_WIDTH; // Offset based on direction
	CGameObject* add_object = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECK_FALL, x + offsetX, y, 0, 0);
	AddCheck(add_object);

	if (checkFall != NULL)
	{
		/*int state = (vx < 0) ? STATE_LEFT_KOOPA : STATE_RIGHT_KOOPA;*/
		/*checkFall->SetState(state);*/
		checkFall->SETay(0.00009f);
	}
}

void CRedKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SHELL:
		vx = 0;
		vy = 0;
		shell_start = GetTickCount64();
		break;
	case KOOPA_STATE_SHELL_FAST_MOVING_LEFT:
		vx = -KOOPA_SHELL_FAST_MOVING_SPEED;
		vy = 0;
		break;
	case KOOPA_STATE_SHELL_FAST_MOVING_RIGHT:
		vx = KOOPA_SHELL_FAST_MOVING_SPEED;
		vy = 0;
		break;
	case KOOPA_STATE_SHELL_HOLD:
		break;
	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		break;
	}
}