#include "j1FlyingEnemy.h"
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

j1FlyingEnemy::j1FlyingEnemy(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::FLYINGENEMY)
{
	animation = NULL;

	flying.LoadAnimationsEnemies("FlyingEnemy_flying");

	initPos.x = position.x = x;
	initPos.y = position.y = y;
}

j1FlyingEnemy::~j1FlyingEnemy() {}

bool j1FlyingEnemy::Start()
{

	sprites = App->tex->Load("textures/enemy2.png");
	Sleeping();
	animation = &flying;
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, colliderSize.x, colliderSize.y }, ENEMY_COLLIDER, App->enty);

	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	collider->Set_Pos(position.x, position.y);



	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(sprites);
	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}

void j1FlyingEnemy::OnCollision(Collider * col_1, Collider * col_2)
{
}

bool j1FlyingEnemy::Load(pugi::xml_node &)
{
	return true;
}

bool j1FlyingEnemy::Save(pugi::xml_node& data)const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

void j1FlyingEnemy::Sleeping()
{
	pugi::xml_document config_file;
	config_file.load_file("config.xml");
	pugi::xml_node config;
	config = config_file.child("config");
	pugi::xml_node harpy;
	harpy = config.child("harpy");

	speed = harpy.attribute("speed").as_int();

	colliderSize.x = harpy.child("colliderSize").attribute("w").as_int();
	colliderSize.y = harpy.child("colliderSize").attribute("h").as_int();
}

void j1FlyingEnemy::fly(p2DynArray<iPoint>& path, float dt)
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