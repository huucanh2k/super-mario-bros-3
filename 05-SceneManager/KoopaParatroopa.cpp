#include "KoopaParatroopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "Koopa.h"

CKoopaParatroopa::CKoopaParatroopa(float x, float y, int model) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = KOOPA_PARATROOPA_GRAVITY;
    this->model = model;
    this->die_start = -1;
    this->shell_start = -1;
    this->jump_start = 0;
    this->isWinged = true;
    this->isOnPlatform = false;
    this->hopCount = 0;
    this->startY = y;  // Save initial Y position for red paratroopa

    // Different initial state based on model
    if (model == KOOPA_PARATROOPA_MODEL_RED) {
        SetState(KOOPA_PARATROOPA_STATE_FLYING_UP);
        this->ay = 0;  // No gravity for flying red paratroopa
    } else {
        SetState(KOOPA_PARATROOPA_STATE_WALKING);
    }
}

void CKoopaParatroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == KOOPA_PARATROOPA_STATE_DIE)
    {
        left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
        top = y - KOOPA_PARATROOPA_BBOX_HEIGHT_DIE / 2;
        right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
        bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT_DIE;
        return;
    }

    if (state == KOOPA_PARATROOPA_STATE_SHELL || state == KOOPA_PARATROOPA_STATE_SHELL_HOLD)
    {
        left = x - KOOPA_PARATROOPA_SHELL_BBOX_WIDTH / 2;
        top = y - KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT / 2;
        right = left + KOOPA_PARATROOPA_SHELL_BBOX_WIDTH;
        bottom = top + KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT;
        return;
    }

    if (isWinged)
    {
        left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
        top = y - KOOPA_PARATROOPA_BBOX_HEIGHT_WINGED / 2;
        right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
        bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT_WINGED;
    }
    else
    {
        left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
        top = y - KOOPA_PARATROOPA_BBOX_HEIGHT / 2;
        right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
        bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT;
    }
}

void CKoopaParatroopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CKoopaParatroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking())
        return;
    
    if (dynamic_cast<CKoopa*>(e->obj))
    {
        OnCollisionWithKoopa(e);
        return;
    }

    if (state == KOOPA_PARATROOPA_STATE_SHELL_MOVING) {
		if (e->nx != 0 && e->obj->IsBlocking()) {
			vx = -vx;
		}

		if (dynamic_cast<CQuestionBrick*>(e->obj)) {
			OnCollisionWithBrick(e);
		}	
	}

    if (e->ny != 0)
    {
        vy = 0;
        
        if (e->ny > 0) {
            isOnPlatform = true;
            
            // For green paratroopa, detect landing on platform
            if (model == KOOPA_PARATROOPA_MODEL_GREEN && isWinged) {
                if (state == KOOPA_PARATROOPA_STATE_FALLING) {
                    SetState(KOOPA_PARATROOPA_STATE_WALKING);
                    DebugOut(L">>> Paratroopa landed on platform >>> \n");
                }
            }
        }
    }
    else if (e->nx != 0)
    {
        if (e->obj->IsBlocking()) {
            if (state == KOOPA_PARATROOPA_STATE_SHELL_MOVING)
            {
                SetState(KOOPA_PARATROOPA_STATE_SHELL_MOVING);
                vx = -vx;  // Reverse direction upon collision
            }
            else if (model == KOOPA_PARATROOPA_MODEL_GREEN && isWinged && 
                    (state == KOOPA_PARATROOPA_STATE_WALKING ||
                     state == KOOPA_PARATROOPA_STATE_JUMPING ||
                     state == KOOPA_PARATROOPA_STATE_FALLING)) {
                vx = -vx;  // Reverse direction for green paratroopa when hitting a wall
            }
        }
    }
}

void CKoopaParatroopa::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	questionBrick->OnCollisionWith(e);
}

void CKoopaParatroopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
    if (koopa->GetState() == KOOPA_STATE_SHELL_MOVE)
    {
        SetState(KOOPA_PARATROOPA_STATE_DIE);
    }
}

void CKoopaParatroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;
    
    // Reset isOnPlatform at the beginning of each update
    isOnPlatform = false;
    
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    // Handle timeouts
    if ((state == KOOPA_PARATROOPA_STATE_DIE) && 
        (GetTickCount64() - die_start > KOOPA_PARATROOPA_DIE_TIMEOUT))
    {
        isDeleted = true;
        return;
    }
    
    // if ((state == KOOPA_PARATROOPA_STATE_SHELL || state == KOOPA_PARATROOPA_STATE_SHELL_HOLD) && 
    //     (GetTickCount64() - shell_start > KOOPA_PARATROOPA_SHELL_TIMEOUT))
    // {
    //     SetState(KOOPA_PARATROOPA_STATE_WALKING);
    //     isWinged = false;  // Once it comes out of shell, it loses wings
        
    //     if (mario->GetIsHolding()) {
    //         mario->SetIsHolding(false);
    //         mario->SetIsHoldingShell(false);
    //     }
    //     return;
    // }

    // Handle model-specific behavior
    if (isWinged) {
        if (model == KOOPA_PARATROOPA_MODEL_GREEN) {
            // Green Paratroopa jumps when on platform
            if (state == KOOPA_PARATROOPA_STATE_WALKING && isOnPlatform) {
                if (GetTickCount64() - jump_start > KOOPA_PARATROOPA_JUMP_TIMEOUT) {
                        SetState(KOOPA_PARATROOPA_STATE_JUMPING);
                        hopCount = 0;
                        DebugOut(L">>> Green Paratroopa Jumping >>> \n");

                    	jump_start = GetTickCount64();
                }
            }
            else if (state == KOOPA_PARATROOPA_STATE_JUMPING) {
                // If reached peak of jump (velocity becomes positive), switch to falling state
                if () {
                    SetState(KOOPA_PARATROOPA_STATE_FALLING);
                    DebugOut(L">>> Green Paratroopa Falling >>> \n");
                }
            }
        }
        else if (model == KOOPA_PARATROOPA_MODEL_RED) {
            // Red Paratroopa flies up and down within limits at same horizontal position
            if (state == KOOPA_PARATROOPA_STATE_FLYING_UP) {
                if (y <= startY - KOOPA_RED_PARATROOPA_FLY_HEIGHT_MAX) {
                    SetState(KOOPA_PARATROOPA_STATE_FLYING_DOWN);
                    DebugOut(L">>> Red Paratroopa Flying Down >>> \n");
                }
            }
            else if (state == KOOPA_PARATROOPA_STATE_FLYING_DOWN) {
                if (y >= startY - KOOPA_RED_PARATROOPA_FLY_HEIGHT_MIN) {
                    SetState(KOOPA_PARATROOPA_STATE_FLYING_UP);
                    DebugOut(L">>> Red Paratroopa Flying Up >>> \n");
                }
            }
        }
    }

    // // Shell holding logic
    // if (state == KOOPA_PARATROOPA_STATE_SHELL) {
    //     if (mario->GetIsHolding()) {
    //         mario->SetIsHoldingShell(true);
    //     }
    // }

    // if (mario->GetIsHoldingShell()) {
    //     if (state == KOOPA_PARATROOPA_STATE_SHELL) {
    //         SetState(KOOPA_PARATROOPA_STATE_SHELL_HOLD);
    //     }
    //     else if (!mario->GetIsHolding() && state == KOOPA_PARATROOPA_STATE_SHELL_HOLD) {
    //         mario->SetIsHoldingShell(false);
    //     }
    //     else if (state == KOOPA_PARATROOPA_STATE_SHELL_HOLD) {
    //         // Update position to follow Mario
    //         float marioX = mario->GetX();
    //         float marioY = mario->GetY();
    //         SetPosition(marioX + mario->GetNx() * MARIO_BIG_BBOX_WIDTH, marioY - 2);
    //     }
    //     return;
    // }
    // else {
    //     if (state == KOOPA_PARATROOPA_STATE_SHELL_HOLD) {
    //         // Mario released the shell - determine direction to shoot it
    //         int direction = LeftOrRightMarrio();
    //         if (direction == 1) {
    //             vx = -KOOPA_PARATROOPA_SHELL_MOVING_SPEED;
    //         }
    //         else {
    //             vx = KOOPA_PARATROOPA_SHELL_MOVING_SPEED;
    //         }
    //         SetState(KOOPA_PARATROOPA_STATE_SHELL_MOVING);
    //         DebugOut(L">>> Paratroopa shell moving >>> \n");
    //     }
    // }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopaParatroopa::Render()
{
    int aniId = -1;

    if (model == KOOPA_PARATROOPA_MODEL_GREEN) {
        if (isWinged) {
            if (state == KOOPA_PARATROOPA_STATE_WALKING)
                aniId = ID_ANI_GREEN_PARATROOPA_WINGED_WALKING;
            else if (state == KOOPA_PARATROOPA_STATE_JUMPING)
                aniId = ID_ANI_GREEN_PARATROOPA_WINGED_JUMPING;
            else if (state == KOOPA_PARATROOPA_STATE_FALLING)
                aniId = ID_ANI_GREEN_PARATROOPA_WINGED_FALLING;
        }
        else {
            if (state == KOOPA_PARATROOPA_STATE_WALKING)
                aniId = ID_ANI_KOOPA_PARATROOPA_WALKING;
            else if (state == KOOPA_PARATROOPA_STATE_SHELL || state == KOOPA_PARATROOPA_STATE_SHELL_HOLD)
                aniId = ID_ANI_KOOPA_PARATROOPA_SHELL;
            else if (state == KOOPA_PARATROOPA_STATE_DIE)
                aniId = ID_ANI_KOOPA_PARATROOPA_DIE;
            else if (state == KOOPA_PARATROOPA_STATE_SHELL_MOVING)
                aniId = ID_ANI_KOOPA_PARATROOPA_SHELL_MOVING;
        }
    }
    else if (model == KOOPA_PARATROOPA_MODEL_RED) {
        if (isWinged) {
            if (state == KOOPA_PARATROOPA_STATE_FLYING_UP)
                aniId = ID_ANI_RED_PARATROOPA_WINGED_FLYING;
            else if (state == KOOPA_PARATROOPA_STATE_FLYING_DOWN)
                aniId = ID_ANI_RED_PARATROOPA_WINGED_FLYING_DOWN;
        }
        else {
            if (state == KOOPA_PARATROOPA_STATE_WALKING)
                aniId = ID_ANI_KOOPA_PARATROOPA_WALKING;
            else if (state == KOOPA_PARATROOPA_STATE_SHELL || state == KOOPA_PARATROOPA_STATE_SHELL_HOLD)
                aniId = ID_ANI_KOOPA_PARATROOPA_SHELL;
            else if (state == KOOPA_PARATROOPA_STATE_DIE)
                aniId = ID_ANI_KOOPA_PARATROOPA_DIE;
            else if (state == KOOPA_PARATROOPA_STATE_SHELL_MOVING)
                aniId = ID_ANI_KOOPA_PARATROOPA_SHELL_MOVING;
        }
    }

    if (aniId != -1)
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    
    RenderBoundingBox();
}

void CKoopaParatroopa::SetState(int state)
{
    CGameObject::SetState(state);
    
    switch (state)
    {
    case KOOPA_PARATROOPA_STATE_WALKING:
        vx = -KOOPA_PARATROOPA_WALKING_SPEED;
        ay = KOOPA_PARATROOPA_GRAVITY;
        jump_start = GetTickCount64();
        break;
        
    case KOOPA_PARATROOPA_STATE_JUMPING:
        vy = -KOOPA_GREEN_PARATROOPA_JUMP_SPEED;
        break;
        
    case KOOPA_PARATROOPA_STATE_FALLING:
        // Just let gravity handle it
        break;
        
    case KOOPA_PARATROOPA_STATE_FLYING_UP:
        vx = 0;  // No horizontal movement for red paratroopa
        vy = -KOOPA_RED_PARATROOPA_FLY_SPEED;
        ay = 0;  // No gravity
        break;
        
    case KOOPA_PARATROOPA_STATE_FLYING_DOWN:
        vx = 0;  // No horizontal movement for red paratroopa
        vy = KOOPA_RED_PARATROOPA_FLY_SPEED;
        ay = 0;  // No gravity
        break;
        
    case KOOPA_PARATROOPA_STATE_SHELL:
        vx = 0;
        vy = 0;
        shell_start = GetTickCount64();
        break;
        
    case KOOPA_PARATROOPA_STATE_SHELL_MOVING:
        ay = KOOPA_PARATROOPA_GRAVITY;
        // vx is set when transitioning from SHELL_HOLD or based on collision
        break;
        
    case KOOPA_PARATROOPA_STATE_SHELL_HOLD:
        vx = 0;
        vy = 0;
        break;
        
    case KOOPA_PARATROOPA_STATE_DIE:
        die_start = GetTickCount64();
        vx = 0;
        vy = 0;
        ay = 0;
        break;
    }
}

int CKoopaParatroopa::LeftOrRightMarrio() {
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	return -1;
    // if (mario->GetX() < x) return 1;  // Mario is on the left
    // else return -1;  // Mario is on the right
}