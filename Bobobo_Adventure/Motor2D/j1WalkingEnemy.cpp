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

j1WalkingEnemy::j1WalkingEnemy(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::FLYINGENEMY)
{
	animation = NULL;

	flying.LoadAnimationsEnemies("FlyingEnemy_flying");

	initPos.x = position.x = x;
	initPos.y = position.y = y;
}

j1WalkingEnemy::~j1WalkingEnemy() {}

bool j1WalkingEnemy::Start()
{

	sprites = App->tex->Load("textures/enemy2.png");
	Sleeping();
	animation = &flying;
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, 30/*colliderSize.x*/,30/*colliderSize.y*/ }, ENEMY_COLLIDER, App->enty);

	return true;
}

bool j1WalkingEnemy::Update(float dt)
{
	BROFILER_CATEGORY("FlyingEnemy Update", Profiler::Color::Tomato)
		collider->Set_Pos(position.x, position.y);

	/*if ((App->enty->player->position.x - position.x) <= range && (App->enty->player->position.x - position.x) >= -range && App->enty->player->collider->type == PLAYER_COLLIDER)
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
	fly(*path, dt);
	path_created = true;
	}
	}
	else if (path_created)
	path->Clear();*/

	Draw();
	if (dead == true) {
		position.x += 50;
	}

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
	if ((col_1->type == ATTACK_COLLIDER && col_2->type == ENEMY_COLLIDER) || (col_2->type == ATTACK_COLLIDER && col_1->type == ENEMY_COLLIDER))
	{
		dead = true;
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
	pugi::xml_document config_file;
	config_file.load_file("config.xml");
	pugi::xml_node config;
	config = config_file.child("config");
	pugi::xml_node harpy;
	harpy = config.child("harpy");



	//colliderSize.x = harpy.child("colliderSize").attribute("w").as_int();
	//colliderSize.y = harpy.child("colliderSize").attribute("h").as_int();
}

void j1WalkingEnemy::fly(p2DynArray<iPoint>& path, float dt)
{
	/*direction = App->path->CheckDirection(path);

	if (direction == Movement::DOWN_RIGHT)
	{
	position.y += speed * dt;
	position.x += speed * dt;
	}else if (direction == Movement::DOWN_LEFT)
	{
	position.y += speed * dt;
	position.x -= speed * dt;
	}else if (direction == Movement::UP_RIGHT)
	{
	position.y -= speed * dt;
	position.x += speed * dt;
	}else if (direction == Movement::UP_LEFT)
	{
	position.y -= speed * dt;
	position.x -= speed * dt;
	}else if (direction == Movement::DOWN)
	position.y += speed * dt;
	else if (direction == Movement::UP)
	position.y -= speed * dt;
	else if (direction == Movement::RIGHT)
	position.x += speed * dt;
	else if (direction == Movement::LEFT)
	position.x -= speed * dt;*/
}