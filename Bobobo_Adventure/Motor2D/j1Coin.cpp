#include "j1Coin.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "Brofiler/Brofiler.h"

j1Coin::j1Coin(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::COIN)
{
	animation = NULL;

	Coin_anim.LoadAnimationsEnemies("Coin");

	initPos.x = position.x = x;
	initPos.y = position.y = y;
}

j1Coin::~j1Coin() {}

bool j1Coin::Start()
{

	sprites = App->tex->Load("textures/Coin.png");
	
	animation = &Coin_anim;
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, 38,36 }, COIN_COLLIDER, App->enty);

	return true;
}

bool j1Coin::Update(float dt)
{
	BROFILER_CATEGORY("Coin Update", Profiler::Color::Tomato)
	collider->Set_Pos(position.x, position.y);

	SDL_Rect rect = animation->GetCurrentFrame(dt);

	if (sprites != NULL) {
		Draw(false, rect);
	}
		




	return true;
}

bool j1Coin::CleanUp()
{
	App->tex->UnLoad(sprites);
	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}


bool j1Coin::Load(pugi::xml_node & data)
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;

}

bool j1Coin::Save(pugi::xml_node& data)const
{
	pugi::xml_node coin = data.append_child("position3");

	coin.append_attribute("x") = position.x;
	coin.append_attribute("y") = position.y;

	if (App->scene->active == true)
	{
		coin.append_attribute("level") = 1;
	}
	else if (App->scene2->active == true)
	{
		coin.append_attribute("level") = 2;
	}

	return true;
}


void j1Coin::OnCollision(Collider * col_1, Collider * col_2)
{
	if ((col_1->type == PLAYER_COLLIDER && col_2->type == COIN_COLLIDER) || (col_2->type == PLAYER_COLLIDER && col_1->type == COIN_COLLIDER))
	{
		CleanUp();
	}
}

