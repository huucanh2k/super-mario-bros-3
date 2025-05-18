
#include "RedKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "Box.h"
#include "Mario.h"
#include "BrickQuestion.h"
#include "SuperLeaf.h"

CRedKoopa::CRedKoopa(float x, float y, int type) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	shell_start = -1;
	this->type = type;
	checkFall = NULL;
	if (type == RED_KOOPA_TYPE_WINGED)
	{
		SetState(RED_KOOPA_STATE_FLYING);
	}
	else
	{
		SetState(KOOPA_STATE_WALKING_LEFT);
	}
	
	CreateCheckFall();
}

void CRedKoopa::Render()
{
	int ani = ID_ANI_RED_KOOPA_WALKING_LEFT;
	if (type == RED_KOOPA_TYPE_WINGED)
	{
		ani = ID_ANI_RED_KOOPA_WINGED_FLYING;
	}
	else
	{
		if (state == RED_KOOPA_STATE_WALKING_RIGHT)
			ani = ID_ANI_RED_KOOPA_WALKING_RIGHT;
		else if (state == RED_KOOPA_STATE_SHELL || state == RED_KOOPA_STATE_SHELL_HOLD)
			ani = ID_ANI_RED_KOOPA_SHELL;
		else if (state == RED_KOOPA_STATE_DIE)
			ani = ID_ANI_RED_KOOPA_DIE;
		else if (state == RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT || state == RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
			ani = ID_ANI_KOOPA_SHELL_FAST_MOVING;
	}

	if (state == RED_KOOPA_STATE_DIE)
	{
		ani = ID_ANI_RED_KOOPA_DIE;
	}

	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CRedKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == RED_KOOPA_STATE_DIE)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else if (state == RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT || RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else if (state == RED_KOOPA_STATE_SHELL)
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else if (state == RED_KOOPA_STATE_SHELL_HOLD)
	{
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
	if (!e->obj->IsBlocking())
		return;
	if (dynamic_cast<CRedKoopa *>(e->obj))
	{
		if (e->obj->GetState() == RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT || e->obj->GetState() == RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
		{
			SetState(RED_KOOPA_STATE_DIE);
			return;
		}
	};

	if (dynamic_cast<CBrickQuestion *>(e->obj) && (state == RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT || state == RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT))
	{
		OnCollisionWithBrickQuestion(e);
	}

	if (e->ny != 0)
	{
		vy = 0;
	}

	else if (e->nx != 0)
	{
		if (e->obj->IsBlocking())
		{
			if (state == RED_KOOPA_STATE_WALKING_LEFT)
			{
				SetState(RED_KOOPA_STATE_WALKING_RIGHT);
			}
			else if (state == RED_KOOPA_STATE_WALKING_RIGHT)
			{
				SetState(RED_KOOPA_STATE_WALKING_LEFT);
			}
			else if (state == RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT)
			{
				SetState(RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
			}
			else if (state == RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT)
			{
				SetState(RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
			}
		}
	}
}

void CRedKoopa::OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e)
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

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if ((state == RED_KOOPA_STATE_DIE) && (GetTickCount64() - die_start > RED_KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (type == RED_KOOPA_TYPE_NORMAL)
	{
		vy += ay * dt;
		vx += ax * dt;

		if ((state == RED_KOOPA_STATE_SHELL || state == RED_KOOPA_STATE_SHELL_HOLD) && (GetTickCount64() - shell_start > RED_KOOPA_SHELL_TIMEOUT))
		{
			SetState(RED_KOOPA_STATE_WALKING_LEFT);
			float offsetX = -KOOPA_BBOX_WIDTH;
			checkFall->SetPosition(x + offsetX, y);
			mario->SetState(MARIO_STATE_IDLE);
			mario->SetIsHolding(false);
			mario->SetIsHoldingShell(false);
			return;
		}

		if (state == RED_KOOPA_STATE_SHELL)
		{
			if (mario->GetIsHolding())
			{
				mario->SetIsHoldingShell(true);
			}
		}

		if (mario->GetIsHoldingShell())
		{
			if (state == RED_KOOPA_STATE_SHELL)
			{
				SetState(RED_KOOPA_STATE_SHELL_HOLD);
			}
			else if (!mario->GetIsHolding() && state == RED_KOOPA_STATE_SHELL_HOLD)
			{
				mario->SetIsHoldingShell(false);
			}
			else if (state == RED_KOOPA_STATE_SHELL_HOLD)
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
			if (state == RED_KOOPA_STATE_SHELL_HOLD)
			{
				if (LeftOrRightMarrio() == 1)
				{
					SetState(RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
					DebugOut(L">>> Check turn into shell moving right >>> \n");
				}
				else if (LeftOrRightMarrio() == -1)
				{
					SetState(RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
					DebugOut(L">>> Check turn into shell moving left >>> \n");
				}
				mario->SetState(MARIO_STATE_IS_KICKING);
			}
		}

		if (checkFall != NULL && (GetState() != RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT && GetState() != RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT))
		{
			CheckFall();
		}
	} else {
		static float startY = y;  // Store the initial y position
        static bool flyingUp = true;  // Track flying direction
        
        // Check if we've reached the upper or lower limit
        if (y <= startY - RED_KOOPA_MAX_FLY_HEIGHT)
        {
            flyingUp = false;  // Start flying down
        }
        else if (y >= startY - RED_KOOPA_MIN_FLY_HEIGHT)
        {
            flyingUp = true;   // Start flying up
        }
        
        // Set velocity based on flying direction
        vy = flyingUp ? -RED_KOOPA_FLY_SPEED : RED_KOOPA_FLY_SPEED;
        
        // Cancel gravity for flying koopas
        ay = 0;
        
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedKoopa::AddCheck(CGameObject *obj)
{
	if (!dynamic_cast<CCheckFall *>(obj))
		return;
	else if (!checkFall)
	{
		CCheckFall *cfall_obj = dynamic_cast<CCheckFall *>(obj);
		checkFall = cfall_obj;

		DebugOut(L">>> check >>> \n");
	}
}

int CRedKoopa::LeftOrRightMarrio()
{
	CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() < GetX())
		return 1;
	else
		return -1;
}

void CRedKoopa::CheckFall()
{
	float offsetX = (vx < 0) ? -KOOPA_BBOX_WIDTH : KOOPA_BBOX_WIDTH; // Offset based on direction
	checkFall->SetPosition(x + offsetX, y);

	// Check if checkFall is no longer on a platform
	if (checkFall->IsFalling())
	{
		// Turn Koopa before falling
		if (GetState() == RED_KOOPA_STATE_WALKING_LEFT)
		{
			SetState(RED_KOOPA_STATE_WALKING_RIGHT);
			checkFall->SetPosition(x + -offsetX, y);
			DebugOut(L">>> check turn right >>> \n");
		}
		else if (RED_KOOPA_STATE_WALKING_RIGHT)
		{
			SetState(RED_KOOPA_STATE_WALKING_LEFT);
			checkFall->SetPosition(x + -offsetX, y);
			DebugOut(L">>> check turn left >>> \n");
		}
	}
}

void CRedKoopa::CreateCheckFall()
{
	CPlayScene *scene = (CPlayScene *)CGame::GetInstance()->GetCurrentScene();

	float offsetX = (vx < 0) ? -KOOPA_BBOX_WIDTH : KOOPA_BBOX_WIDTH; // Offset based on direction
	CGameObject *add_object = scene->CreateObjectAndReturn(OBJECT_TYPE_CHECK_FALL, x + offsetX, y, 0, 0);
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
	case RED_KOOPA_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case RED_KOOPA_STATE_FLYING:
		vx = 0;
		vy = 0;
		break;
	case RED_KOOPA_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		break;
	case RED_KOOPA_STATE_SHELL:
		vx = 0;
		vy = 0;
		shell_start = GetTickCount64();
		break;
	case RED_KOOPA_STATE_SHELL_FAST_MOVING_LEFT:
		vx = -KOOPA_SHELL_FAST_MOVING_SPEED;
		vy = 0;
		break;
	case RED_KOOPA_STATE_SHELL_FAST_MOVING_RIGHT:
		vx = KOOPA_SHELL_FAST_MOVING_SPEED;
		vy = 0;
		break;
	case RED_KOOPA_STATE_SHELL_HOLD:
		break;
	case RED_KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		break;
	}
}