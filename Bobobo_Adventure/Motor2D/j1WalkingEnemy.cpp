#include "j1WalkingEnemy.h"
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
#include "Brofiler/Brofiler.h"

j1WalkingEnemy::j1WalkingEnemy(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::WALKINGENEMY)
{
	animation = NULL;

	idle.LoadAnimationsEnemies("WalkingEnemy_idle");
	walking.LoadAnimationsEnemies("WalkingEnemy_walk");

	initPos.x = position.x = x;
	initPos.y = position.y = y;
}

j1WalkingEnemy::~j1WalkingEnemy() {}

bool j1WalkingEnemy::Start()
{

	sprites = App->tex->Load("textures/enemy1_3.png");
	Sleeping();
	animation = &idle;
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, 55,60 }, ENEMY_COLLIDER, App->enty);

	return true;
}

bool j1WalkingEnemy::Update(float dt)
{
	BROFILER_CATEGORY("FlyingEnemy Update", Profiler::Color::Tomato)
		collider->Set_Pos(position.x, position.y);

	position.y += GRAVITY + GRAVITY * dt;

	if (path_created) {
		if ((App->enty->player->position.x - position.x) <= range && (App->enty->player->position.x - position.x) >= -range && App->enty->player->collider->type == PLAYER_COLLIDER)
		{
			iPoint origin = { App->map->WorldToMap((int)position.x + colliderSize.x / 2, (int)position.y + colliderSize.y / 2) };
			iPoint destination;
			if (position.x < App->enty->player->position.x)
				destination = { App->map->WorldToMap((int)App->enty->player->position.x + App->enty->player->playerwidth + 1, (int)App->enty->player->position.y + App->enty->player->playerheight / 2) };
			else
				destination = { App->map->WorldToMap((int)App->enty->player->position.x, (int)App->enty->player->position.y + App->enty->player->playerheight / 2) };

			if (!App->enty->player->dead && App->path->IsWalkable(destination) && App->path->IsWalkable(origin))
			{
				path = App->path->CreatePath(origin, destination);
				walk(*path, dt);
				path_created = true;
			}
		}
		else if (path_created)
			path->Clear();
	}
	if (App->enty->player->position == App->enty->player->Initial_position)
	{
		animation = &idle;
		position = initPos;
	}
		

	else if (path_created)
		path->Clear();

	Draw();

	return true;
}

bool j1WalkingEnemy::CleanUp()
{
	App->tex->UnLoad(sprites);
	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}

void j1WalkingEnemy::OnCollision(Collider * col_1, Collider * col_2)
{

	COLLISION_DIRECTION direction;

	if ((col_1->type == GROUND_COLLIDER && col_2->type == ENEMY_COLLIDER) || (col_2->type == GROUND_COLLIDER && col_1->type == ENEMY_COLLIDER))
	{
		direction = col_1->CheckDirection(col_2->rect);

		if (direction == UP_COLLISION)
			position.y = col_2->rect.y - colliderSize.y - 1;

		else if (direction == DOWN_COLLISION)
			position.y = col_2->rect.y + col_2->rect.h;

		else if (direction == RIGHT_COLLISION)
			position.x = col_2->rect.x + col_2->rect.w;

		else if (direction == LEFT_COLLISION)
			position.x = col_2->rect.x - colliderSize.x;
	}

	if ((col_1->type == ATTACK_COLLIDER && col_2->type == ENEMY_COLLIDER) || (col_2->type == ATTACK_COLLIDER && col_1->type == ENEMY_COLLIDER))
	{
		CleanUp();
	}
}

bool j1WalkingEnemy::Load(pugi::xml_node &)
{
	return true;
}

bool j1WalkingEnemy::Save(pugi::xml_node& data)const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

void j1WalkingEnemy::Sleeping()
{
}

void j1WalkingEnemy::walk(p2DynArray<iPoint>& path, float dt)
{
	direction = App->path->CheckDirectionGround(path);

	speed = 100.0f;
	if (direction == Movement::DOWN)
	{
		animation = &walking;
		position.y += speed * dt;
	} else if (direction == Movement::RIGHT)
	{
		animation = &walking;
		position.x += speed * dt;
	} else if (direction == Movement::LEFT)
	{
		animation = &walking;
		position.x -= speed * dt;
	}
		
}