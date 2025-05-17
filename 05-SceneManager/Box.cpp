#include "Box.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBox::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->rowNum * PER_BOX_WIDTH / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, 1.0f, rect.right - 1, rect.bottom - 1);
}

void CBox::Render()
{
	int aniId = ID_ANI_BOX;
	
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
	
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->rowNum * PER_BOX_WIDTH / 2;
	float cellHeight_div_2 = this->columnNum * PER_BOX_HEIGHT / 2;
	l = x - cellWidth_div_2;
	t = y - cellHeight_div_2;
	r = l + this->rowNum * PER_BOX_WIDTH;
	b = t + this->columnNum * PER_BOX_HEIGHT;
}

int CBox::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	//if (nx != 0 && ny == 0) return 1;
	else return 0;
}