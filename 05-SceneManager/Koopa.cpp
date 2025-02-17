#include "Koopa.h"
#include "Goomba.h"

CKoopa::CKoopa(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	shell_start = -1;
	SetState(KOOPA_STATE_WALKING_LEFT);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) {
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
		if (state == KOOPA_STATE_WALKING_LEFT) {
			SetState(KOOPA_STATE_WALKING_RIGHT);
		}
		else if (state == KOOPA_STATE_WALKING_RIGHT) {
			SetState(KOOPA_STATE_WALKING_LEFT);
		}
		else if (state == KOOPA_STATE_SHELL_FAST_MOVING_LEFT) {
			SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
		}
		else if (state == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT) {
			SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
		}
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
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

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	int ani = ID_ANI_KOOPA_WALKING_LEFT;
	if (state == KOOPA_STATE_WALKING_RIGHT)
		ani = ID_ANI_KOOPA_WALKING_RIGHT;
	else if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_HOLD)
		ani = ID_ANI_KOOPA_SHELL;
	else if (state == KOOPA_STATE_DIE)
		ani = ID_ANI_KOOPA_DIE;
	else if (state == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || state == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
		ani = ID_ANI_KOOPA_SHELL_FAST_MOVING;
	
	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CRed_Koopa::CreateCheckfallSmall() {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();



	if (vx < 0)
	{
		CGameObject* add_object_left1 = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA_ON_GLASS_BRICK, GetX() - 1, y + 6, 0, 0);
		AddCheck(add_object_left1);
		DebugOut(L">>> check tao obj left >>> \n");
		checkfall->SetState(SMALL_STATE_LEFT_KOOPA);
		//checkfall->SETay(0.0009f);


	}
	else if (vx >= 0)
	{
		CGameObject* add_object_right1 = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA_ON_GLASS_BRICK, GetX() + 1, y + 6, 0/* KOOPA_RED_WALKING_SPEED*/, 0);

		AddCheck(add_object_right1);
		DebugOut(L">>> check tao obj right >>> \n");
		checkfall->SetState(SMALL_STATE_RIGHT_KOOPA);
		//checkfall->SETay(0.0009f);

	}




}

void CRed_Koopa::CreateCheckfall() {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (vx < 0)
	{
		CGameObject* add_object_left = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA, GetX() - KOOPA_RED_BBOX_WIDTH, y + 1, 0, 0);
		AddCheck(add_object_left);
		DebugOut(L">>> check tao obj left >>> \n");
		checkfall->SetState(STATE_LEFT_KOOPA);
		checkfall->SETay(0.00009f);


	}
	else
	{
		CGameObject* add_object_right = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA, GetX() + KOOPA_RED_BBOX_WIDTH, y + 1, 0/* KOOPA_RED_WALKING_SPEED*/, 0);

		AddCheck(add_object_right);
		DebugOut(L">>> check tao obj right >>> \n");
		checkfall->SetState(STATE_RIGHT_KOOPA);
		checkfall->SETay(0.00009f);

	}

}
void CRed_Koopa::CreateCheckfallSmall() {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();



	if (vx < 0)
	{
		CGameObject* add_object_left1 = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA_ON_GLASS_BRICK, GetX() - 1, y + 6, 0, 0);
		AddCheck(add_object_left1);
		DebugOut(L">>> check tao obj left >>> \n");
		checkfall->SetState(SMALL_STATE_LEFT_KOOPA);
		//checkfall->SETay(0.0009f);


	}
	else if (vx >= 0)
	{
		CGameObject* add_object_right1 = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA_ON_GLASS_BRICK, GetX() + 1, y + 6, 0/* KOOPA_RED_WALKING_SPEED*/, 0);

		AddCheck(add_object_right1);
		DebugOut(L">>> check tao obj right >>> \n");
		checkfall->SetState(SMALL_STATE_RIGHT_KOOPA);
		//checkfall->SETay(0.0009f);

	}
}

void CRed_Koopa::CreateCheckfall() {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (vx < 0)
	{
		CGameObject* add_object_left = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA, GetX() - KOOPA_RED_BBOX_WIDTH, y + 1, 0, 0);
		AddCheck(add_object_left);
		DebugOut(L">>> check tao obj left >>> \n");
		checkfall->SetState(STATE_LEFT_KOOPA);
		checkfall->SETay(0.00009f);


	}
	else
	{
		CGameObject* add_object_right = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECKFALL_KOOPA, GetX() + KOOPA_RED_BBOX_WIDTH, y + 1, 0/* KOOPA_RED_WALKING_SPEED*/, 0);

		AddCheck(add_object_right);
		DebugOut(L">>> check tao obj right >>> \n");
		checkfall->SetState(STATE_RIGHT_KOOPA);
		checkfall->SETay(0.00009f);

	}

}

int CGreen_Koopa::LeftOrRightMarrio() {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() < GetX()) return 1;
	else return -1;
}


void CKoopa::SetState(int state)
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