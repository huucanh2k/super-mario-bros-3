#include "WingedKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "Koopa.h"
#include "Platform.h"
#include "BoxPlatform.h"

CWingedKoopa::CWingedKoopa(float x, float y, int model) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = WINGED_KOOPA_GRAVITY;
    this->model = model;
    this->die_start = -1;
    this->shell_start = -1;
    this->stateShakingStart = -1;
    this->jump_start = 0;
    this->isWinged = true;
    this->isOnPlatform = false;
    this->hopCount = 0;
    this->startY = y; // Save initial Y position for red paratroopa

    // Different initial state based on model
    if (model == WINGED_KOOPA_MODEL_RED)
    {
        SetState(WINGED_KOOPA_STATE_FLYING_UP);
        this->ay = 0; // No gravity for flying red paratroopa
    }
    else
    {
        SetState(WINGED_KOOPA_STATE_WALKING);
    }
    DebugOut(L"[INFO] Winged Koopa created at (%f, %f)\n", x, y);
}

void CWingedKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
    if (state == WINGED_KOOPA_STATE_DIE)
    {
        left = x - WINGED_KOOPA_BBOX_WIDTH / 2;
        top = y - WINGED_KOOPA_BBOX_HEIGHT_DIE / 2;
        right = left + WINGED_KOOPA_BBOX_WIDTH;
        bottom = top + WINGED_KOOPA_BBOX_HEIGHT_DIE;
        return;
    }

    if (state == WINGED_KOOPA_STATE_SHELL || state == WINGED_KOOPA_STATE_SHELL_HOLD)
    {
        left = x - WINGED_KOOPA_SHELL_BBOX_WIDTH / 2;
        top = y - WINGED_KOOPA_SHELL_BBOX_HEIGHT / 2;
        right = left + WINGED_KOOPA_SHELL_BBOX_WIDTH;
        bottom = top + WINGED_KOOPA_SHELL_BBOX_HEIGHT;
        return;
    }

    if (isWinged)
    {
        left = x - WINGED_KOOPA_BBOX_WIDTH / 2;
        top = y - WINGED_KOOPA_BBOX_HEIGHT_WINGED / 2;
        right = left + WINGED_KOOPA_BBOX_WIDTH;
        bottom = top + WINGED_KOOPA_BBOX_HEIGHT_WINGED;
    }
    else
    {
        left = x - WINGED_KOOPA_BBOX_WIDTH / 2;
        top = y - WINGED_KOOPA_BBOX_HEIGHT / 2;
        right = left + WINGED_KOOPA_BBOX_WIDTH;
        bottom = top + WINGED_KOOPA_BBOX_HEIGHT;
    }
}

void CWingedKoopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
    // When in free fall (no collision), the koopa is not on platform
    isOnPlatform = false;
}

void CWingedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking())
        return;

    if (dynamic_cast<CKoopa *>(e->obj))
    {
        OnCollisionWithKoopa(e);
        return;
    }

    // Check if colliding with a platform
    bool isPlatform = false;
    if (dynamic_cast<CPlatform *>(e->obj) || 
        dynamic_cast<CBoxPlatform *>(e->obj))
    {
        isPlatform = true;
    }

    if (state == WINGED_KOOPA_STATE_SHELL_MOVING)
    {
        if (e->nx != 0 && e->obj->IsBlocking())
        {
            vx = -vx;
        }

        if (dynamic_cast<CQuestionBrick *>(e->obj))
        {
            OnCollisionWithBrick(e);
        }
    }

    if (e->ny != 0)
    {
        vy = 0;
        // Check for platform collision: the koopa is landing on top of a platform when ny < 0
        if (e->ny < 0)
        {
            isOnPlatform = true;
            
            // If we're falling and land on a platform, switch to walking
            if (state == WINGED_KOOPA_STATE_FALLING && model == WINGED_KOOPA_MODEL_GREEN && isWinged)
            {
                SetState(WINGED_KOOPA_STATE_WALKING);
            }
            
            DebugOut(L">>> Winged Koopa on platform, state: %d >>> \n", state);
        }
    }
    else if (e->nx != 0)
    {
        if (e->obj->IsBlocking())
        {
            if (state == WINGED_KOOPA_STATE_SHELL_MOVING)
            {
                SetState(WINGED_KOOPA_STATE_SHELL_MOVING);
                vx = -vx; // Reverse direction upon collision
                DebugOut(L">>> Shell hit wall, reversing direction >>> \n");
            }
            else if (model == WINGED_KOOPA_MODEL_GREEN && isWinged &&
                     (state == WINGED_KOOPA_STATE_WALKING ||
                      state == WINGED_KOOPA_STATE_JUMPING ||
                      state == WINGED_KOOPA_STATE_FALLING))
            {
                vx = -vx; // Reverse direction for green paratroopa when hitting a wall
                DebugOut(L">>> Green Paratroopa hit wall, reversing direction >>> \n");
            }
        }
    }
}

void CWingedKoopa::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
    CQuestionBrick *questionBrick = dynamic_cast<CQuestionBrick *>(e->obj);
    questionBrick->OnCollisionWith(e);
}

void CWingedKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa *koopa = dynamic_cast<CKoopa *>(e->obj);
    if (koopa->GetState() == KOOPA_STATE_SHELL_MOVE)
    {
        SetState(WINGED_KOOPA_STATE_DIE);
    }
}

void CWingedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    // Reset isOnPlatform at the beginning of each update
    // Will be set to true in collision handling if koopa lands on platform

    CMario *mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    // Handle timeouts
    if ((state == WINGED_KOOPA_STATE_DIE) &&
        (GetTickCount64() - die_start > WINGED_KOOPA_DIE_TIMEOUT))
    {
        isDeleted = true;
        return;
    }

    // Shell shaking and timeout logic
    if (state == WINGED_KOOPA_STATE_SHELL &&
        (GetTickCount64() - shell_start > WINGED_KOOPA_SHELL_TIMEOUT))
    {
        SetState(WINGED_KOOPA_STATE_SHELL_SHAKING);
        // DebugOut(L">>> Koopa shell starts shaking >>> \n");
        return;
    }

    DebugOut(L">>> Winged Koopa State: %d, vy: %f, isOnPlatform: %d >>> \n", 
             state, vy, isOnPlatform);

    // Handle model-specific behavior
    if (isWinged)
    {
        if (model == WINGED_KOOPA_MODEL_GREEN)
        {
            // Start jumping when below original Y position
            if (state == WINGED_KOOPA_STATE_WALKING && isOnPlatform) // Jump when below original Y position
            {
                SetState(WINGED_KOOPA_STATE_JUMPING);
                DebugOut(L">>> Green Paratroopa jumping: y=%f, startY=%f >>> \n", y, startY);
            }
            else if (state == WINGED_KOOPA_STATE_JUMPING)
            { 
                if (GetTickCount64() - jump_start > WINGED_KOOPA_JUMP_TIMEOUT)
                {
                    SetState(WINGED_KOOPA_STATE_FALLING);
                    DebugOut(L">>> Green Paratroopa reached peak, switching to falling >>> \n");
                }
            } 
            else if (state == WINGED_KOOPA_STATE_FALLING && isOnPlatform)
            {
                // If landed on platform, switch to walking state
                SetState(WINGED_KOOPA_STATE_JUMPING);
                DebugOut(L">>> Green Paratroopa landed, switching to walking >>> \n");
            }
        }
        else if (model == WINGED_KOOPA_MODEL_RED)
        {
            // Red Paratroopa flies up and down within limits at same horizontal position
            if (state == WINGED_KOOPA_STATE_FLYING_UP)
            {
                if (y <= startY - KOOPA_RED_PARATROOPA_FLY_HEIGHT_MAX)
                {
                    SetState(WINGED_KOOPA_STATE_FLYING_DOWN);
                    // DebugOut(L">>> Red Paratroopa Flying Down >>> \n");
                }
            }
            else if (state == WINGED_KOOPA_STATE_FLYING_DOWN)
            {
                if (y >= startY - KOOPA_RED_PARATROOPA_FLY_HEIGHT_MIN)
                {
                    SetState(WINGED_KOOPA_STATE_FLYING_UP);
                    // DebugOut(L">>> Red Paratroopa Flying Up >>> \n");
                }
            }
        }
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CWingedKoopa::Render()
{
    int aniId = -1;

    if (model == WINGED_KOOPA_MODEL_GREEN)
    {
        if (isWinged)
        {
            if (state == WINGED_KOOPA_STATE_WALKING)
                aniId = ID_ANI_GREEN_WINGED_KOOPA_WALKING;
            else if (state == WINGED_KOOPA_STATE_JUMPING)
                aniId = ID_ANI_GREEN_WINGED_KOOPA_JUMPING;
            else if (state == WINGED_KOOPA_STATE_FALLING)
                aniId = ID_ANI_GREEN_WINGED_KOOPA_FALLING;
        }
        else
        {
            if (state == WINGED_KOOPA_STATE_WALKING)
                aniId = ID_ANI_WINGED_KOOPA_WALKING;
            else if (state == WINGED_KOOPA_STATE_SHELL || state == WINGED_KOOPA_STATE_SHELL_HOLD)
                aniId = ID_ANI_WINGED_KOOPA_SHELL;
            else if (state == WINGED_KOOPA_STATE_DIE)
                aniId = ID_ANI_WINGED_KOOPA_DIE;
            else if (state == WINGED_KOOPA_STATE_SHELL_MOVING)
                aniId = ID_ANI_WINGED_KOOPA_SHELL_MOVING;
            else if (state == WINGED_KOOPA_STATE_SHELL_SHAKING)
                aniId = ID_ANI_WINGED_KOOPA_SHELL_SHAKING;
        }
    }
    else if (model == WINGED_KOOPA_MODEL_RED)
    {
        if (isWinged)
        {
            if (state == WINGED_KOOPA_STATE_FLYING_UP)
                aniId = ID_ANI_RED_WINGED_KOOPA_FLYING;
            else if (state == WINGED_KOOPA_STATE_FLYING_DOWN)
                aniId = ID_ANI_RED_WINGED_KOOPA_FLYING_DOWN;
        }
        else
        {
            if (state == WINGED_KOOPA_STATE_WALKING)
                aniId = ID_ANI_WINGED_KOOPA_WALKING;
            else if (state == WINGED_KOOPA_STATE_SHELL || state == WINGED_KOOPA_STATE_SHELL_HOLD)
                aniId = ID_ANI_WINGED_KOOPA_SHELL;
            else if (state == WINGED_KOOPA_STATE_DIE)
                aniId = ID_ANI_WINGED_KOOPA_DIE;
            else if (state == WINGED_KOOPA_STATE_SHELL_MOVING)
                aniId = ID_ANI_WINGED_KOOPA_SHELL_MOVING;
            else if (state == WINGED_KOOPA_STATE_SHELL_SHAKING)
                aniId = ID_ANI_WINGED_KOOPA_SHELL_SHAKING;
        }
    }

    LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
    if (ani == nullptr)
    {
        DebugOut(L"[ERROR] Animation %d not found\n", aniId);
        return;
    }

    if (aniId != -1)
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);

    RenderBoundingBox();
}

void CWingedKoopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case WINGED_KOOPA_STATE_WALKING:
        if (model == WINGED_KOOPA_MODEL_GREEN) {
            // If no specific direction, default to left movement
            if (vx == 0) {
                vx = -WINGED_KOOPA_WALKING_SPEED;
            }
            // Maintain current direction if already moving
        }
        ay = WINGED_KOOPA_GRAVITY;
        jump_start = GetTickCount64();
        break;

    case WINGED_KOOPA_STATE_JUMPING:
        vy = -KOOPA_GREEN_PARATROOPA_JUMP_SPEED;
        ay = 0;
        jump_start = GetTickCount64();
        break;

    case WINGED_KOOPA_STATE_FALLING:
        // Set a small initial falling velocity to ensure the koopa starts falling
        vy = KOOPA_GREEN_PARATROOPA_FALL_SPEED;
        ay = 0; // Ensure gravity is applied during falling
        break;

    case WINGED_KOOPA_STATE_FLYING_UP:
        vx = 0; // No horizontal movement for red paratroopa
        vy = -KOOPA_RED_PARATROOPA_FLY_SPEED;
        ay = 0; // No gravity
        break;

    case WINGED_KOOPA_STATE_FLYING_DOWN:
        vx = 0; // No horizontal movement for red paratroopa
        vy = KOOPA_RED_PARATROOPA_FLY_SPEED;
        ay = 0; // No gravity
        break;

    case WINGED_KOOPA_STATE_SHELL:
        vx = 0;
        vy = 0;
        shell_start = GetTickCount64();
        break;

    case WINGED_KOOPA_STATE_SHELL_MOVING:
        ay = WINGED_KOOPA_GRAVITY;
        // vx is set when transitioning from SHELL_HOLD or based on collision
        break;

    case WINGED_KOOPA_STATE_SHELL_HOLD:
        vx = 0;
        vy = 0;
        break;

    case WINGED_KOOPA_STATE_SHELL_SHAKING:
        vx = 0;
        vy = 0;
        stateShakingStart = GetTickCount64();
        DebugOut(L"[INFO] Koopa shell shaking start\n");
        break;

    case WINGED_KOOPA_STATE_DIE:
        die_start = GetTickCount64();
        vx = 0;
        vy = 0;
        ay = 0;
        break;
    }
}