#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

bool CGameObject::IsInScreen() {
	float cam_x, cam_y;
	int offSet = 20;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	if (x < cam_x - offSet || x > cam_x + SCREEN_WIDTH + offSet || y < cam_y - offSet || y > cam_y + SCREEN_HEIGHT + offSet)
		return false;
	return true;
}

CGameObject::~CGameObject()
{

}