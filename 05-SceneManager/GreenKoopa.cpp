
#include "GreenKoopa.h"
#include "RedKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "Box.h"
#include "Mario.h"
#include "BrickQuestion.h"
#include "SuperLeaf.h"

CGreenKoopa::CGreenKoopa(float x, float y, int type) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GREEN_KOOPA_GRAVITY;
	this->type = type;
	this->jump_start = -1;
	this->die_start = -1;
	this->shell_start = -1;
	this->isOnPlatform = true;
	SetState(GREEN_KOOPA_STATE_WALKING);
}

void CGreenKoopa::Render()
{
	int ani = ID_ANI_GREEN_KOOPA_WALKING;

	if (type == GREEN_KOOPA_TYPE_WINGED)
	{
		if (state == GREEN_KOOPA_STATE_WALKING)
			ani = ID_ANI_GREEN_KOOPA_WINGED_WALKING;
		else if (state == GREEN_KOOPA_STATE_JUMPING)
			ani = ID_ANI_GREEN_KOOPA_WINGED_JUMPING;
		else if (state == GREEN_KOOPA_STATE_FALLING)
			ani = ID_ANI_GREEN_KOOPA_WINGED_FALLING;
	}
	else
	{
		if (state == GREEN_KOOPA_STATE_WALKING)
			ani = ID_ANI_GREEN_KOOPA_WALKING;
		else if (state == GREEN_KOOPA_STATE_SHELL || state == GREEN_KOOPA_STATE_SHELL_HOLD)
			ani = ID_ANI_GREEN_KOOPA_SHELL;
		else if (state == GREEN_KOOPA_STATE_DIE)
			ani = ID_ANI_GREEN_KOOPA_DIE;
		else if (state == GREEN_KOOPA_STATE_SHELL_MOVING)
			ani = ID_ANI_GREEN_KOOPA_SHELL_MOVING;
	}

	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CGreenKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

	if (state == GREEN_KOOPA_STATE_DIE)
	{
		left = x - GREEN_KOOPA_BBOX_WIDTH / 2;
		top = y - GREEN_KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + GREEN_KOOPA_BBOX_WIDTH;
		bottom = top + GREEN_KOOPA_BBOX_HEIGHT_DIE;
		return;
	}

	if (state == GREEN_KOOPA_STATE_SHELL || state == GREEN_KOOPA_STATE_SHELL_HOLD)
	{
		left = x - GREEN_KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - GREEN_KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + GREEN_KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + GREEN_KOOPA_SHELL_BBOX_HEIGHT;
		return;
	}

	if (type == GREEN_KOOPA_TYPE_WINGED)
	{
		left = x - GREEN_KOOPA_BBOX_WIDTH / 2;
		top = y - GREEN_KOOPA_BBOX_HEIGHT_WINGED / 2;
		right = left + GREEN_KOOPA_BBOX_WIDTH;
		bottom = top + GREEN_KOOPA_BBOX_HEIGHT_WINGED;
	}
	else
	{
		left = x - GREEN_KOOPA_BBOX_WIDTH / 2;
		top = y - GREEN_KOOPA_BBOX_HEIGHT / 2;
		right = left + GREEN_KOOPA_BBOX_WIDTH;
		bottom = top + GREEN_KOOPA_BBOX_HEIGHT;
	}
}

void CGreenKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGreenKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking())
		return;
	if (dynamic_cast<CRedKoopa *>(e->obj))
	{
		OnCollisionWithRedKoopa(e);
		return;
	};

	if (dynamic_cast<CBrickQuestion *>(e->obj) && (state == GREEN_KOOPA_STATE_SHELL_MOVING))
	{
		OnCollisionWithBrickQuestion(e);
		return;
	}

	if (e->ny != 0)
	{
		vy = 0;

		isOnPlatform = true;

		// If we're falling and just landed on a platform, immediately go to walking state
		if (state == GREEN_KOOPA_STATE_FALLING && type == GREEN_KOOPA_TYPE_WINGED)
		{
			SetState(GREEN_KOOPA_STATE_WALKING);
			DebugOut(L">>> Green Koopa landed on platform >>> \n");
		}
	}
	else if (e->nx != 0)
	{
		if (e->obj->IsBlocking())
		{
			if (state == GREEN_KOOPA_STATE_SHELL_MOVING)
			{
				vx = -vx;
			}
		}
	}
}

void CGreenKoopa::OnCollisionWithRedKoopa(LPCOLLISIONEVENT e)
{
	CRedKoopa *red_koopa = dynamic_cast<CRedKoopa *>(e->obj);
	if (red_koopa->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_LEFT || red_koopa->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
	{
		SetState(GREEN_KOOPA_STATE_DIE);
	}
}

void CGreenKoopa::OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e)
{
	CBrickQuestion *brick_question = dynamic_cast<CBrickQuestion *>(e->obj);
	CPlayScene *scene = (CPlayScene *)CGame::GetInstance()->GetCurrentScene();
	int x = brick_question->GetX();
	int y = brick_question->GetY();

	// jump on top >> kill Goomba and deflect a bit
	if (e->nx != 0)
	{
		if (!brick_question->GetIsEmpty())
		{
			brick_question->SetState(BRICK_QUESTION_STATE_UP);
			brick_question->SetIsUnbox(true);
			if (brick_question->GetModel() == BRICK_QUESTION_LEAF)
			{
				CSuperLeaf *leafSummon = new CSuperLeaf(x, y - (BRICK_Q_BBOX_HEIGHT - ADJUST_UP_DOWN + 30));
				scene->AddObject(leafSummon);
				leafSummon->SetState(LEAF_SUMMON_STATE);
			}
		}
	}
}

void CGreenKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (type == GREEN_KOOPA_TYPE_WINGED)
	{
		// Add a debug message to track state transitions
		DebugOut(L">>> Green Koopa State: %d, vy: %f, jump_start: %lld, is_On_platform %d>>> \n", state, vy, jump_start, isOnPlatform);

		if (state == GREEN_KOOPA_STATE_WALKING)
		{
			SetState(GREEN_KOOPA_STATE_JUMPING);
			isOnPlatform = false;
			DebugOut(L">>> Green Koopa Jumping >>> \n");
		}
		else if (state == GREEN_KOOPA_STATE_JUMPING)
		{
			vy = -GREEN_KOOPA_JUMP_SPEED;
			if (GetTickCount64() - jump_start > GREEN_KOOPA_JUMP_TIMEOUT)
			{
				SetState(GREEN_KOOPA_STATE_FALLING);
				DebugOut(L">>> Green Koopa Falling >>> \n");
			}
		}
		else if (state == GREEN_KOOPA_STATE_FALLING && isOnPlatform)
		{
			SetState(GREEN_KOOPA_STATE_WALKING);
			DebugOut(L">>> Green Koopa Walking >>> \n");
		}
	}

	if ((state == GREEN_KOOPA_STATE_DIE) && (GetTickCount64() - die_start > GREEN_KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == GREEN_KOOPA_STATE_SHELL || state == GREEN_KOOPA_STATE_SHELL_HOLD) && (GetTickCount64() - shell_start > GREEN_KOOPA_SHELL_TIMEOUT))
	{
		SetState(GREEN_KOOPA_STATE_WALKING);
		float offsetX = -GREEN_KOOPA_BBOX_WIDTH;
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetIsHolding(false);
		mario->SetIsHoldingShell(false);
		return;
	}

	if (state == GREEN_KOOPA_STATE_SHELL)
	{
		if (mario->GetIsHolding())
		{
			mario->SetIsHoldingShell(true);
		}
	}

	if (mario->GetIsHoldingShell())
	{
		if (state == GREEN_KOOPA_STATE_SHELL)
		{
			SetState(GREEN_KOOPA_STATE_SHELL_HOLD);
		}
		else if (!mario->GetIsHolding() && state == GREEN_KOOPA_STATE_SHELL_HOLD)
		{
			mario->SetIsHoldingShell(false);
		}
		else if (state == GREEN_KOOPA_STATE_SHELL_HOLD)
		{
			float marioX, marioY, marioDirection;
			marioX = mario->GetX();
			marioY = mario->GetY();
			SetPosition(marioX + mario->GetNx() * MARIO_BIG_BBOX_WIDTH, marioY - 2);
		}
		return;
	}
	else
	{
		if (state == GREEN_KOOPA_STATE_SHELL_HOLD)
		{
			if (LeftOrRightMarrio() == 1)
			{
				SetState(GREEN_KOOPA_STATE_SHELL_MOVING);
				vx = -GREEN_KOOPA_SHELL_MOVING_SPEED;
				DebugOut(L">>> Check turn into shell moving right >>> \n");
			}
			else if (LeftOrRightMarrio() == -1)
			{
				SetState(GREEN_KOOPA_STATE_SHELL_MOVING);
				vx = GREEN_KOOPA_SHELL_MOVING_SPEED;
				DebugOut(L">>> Check turn into shell moving left >>> \n");
			}
			mario->SetState(MARIO_STATE_IS_KICKING);
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

int CGreenKoopa::LeftOrRightMarrio()
{
	CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() < GetX())
		return 1;
	else
		return -1;
}

void CGreenKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case GREEN_KOOPA_STATE_JUMPING:
		vy = -GREEN_KOOPA_JUMP_SPEED;
		vx = -GREEN_KOOPA_WALKING_SPEED;
		jump_start = GetTickCount64();
		break;
	case GREEN_KOOPA_STATE_FALLING:
		vx = -GREEN_KOOPA_WALKING_SPEED;
		break;
	case GREEN_KOOPA_STATE_SHELL:
		vx = 0;
		vy = 0;
		shell_start = GetTickCount64();
		break;
	case GREEN_KOOPA_STATE_SHELL_MOVING:
		vx = -GREEN_KOOPA_SHELL_MOVING_SPEED;
		vy = 0;
		break;
	case GREEN_KOOPA_STATE_SHELL_HOLD:
		break;
	case GREEN_KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		break;
	}
}