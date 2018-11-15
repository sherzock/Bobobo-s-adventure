#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"



j1EntityManager::j1EntityManager()
{
	name.create("entity");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Start()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (queue[i].type != entitytypes::NO_TYPE)
			{
				CreateEnemy(queue[i]);
				queue[i].type = entitytypes::NO_TYPE;
			}
		}

	return true;
}

bool j1EntityManager::Update(float dt)
{

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->Update(dt);
		}

	return true;
}

bool j1EntityManager::PostUpdate()
{

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->PostUpdate();
		}

	return true;
}

bool j1EntityManager::CleanUp()
{

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->CleanUp();
		int index = entities.find(iterator->data);
		RELEASE(entities.At(index)->data);
		entities.del(entities.At(index));
	}

	player = nullptr;
	
	return true;
}

j1Entity* j1EntityManager::CreateEntity(entitytypes type, int x, int y)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PLAYER:
		ret = new j1Player(x, y, type);
		if (ret != nullptr) entities.add(ret);
		break;

	}
	return ret;
}

void j1EntityManager::AddEnemy(int x, int y, entitytypes type)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == entitytypes::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].position.x = x;
			queue[i].position.y = y;
			break;
		}
	}
}

void j1EntityManager::CreateEnemy(const EnemyInfo& info)
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != entitytypes::NO_TYPE)
		{
			j1Entity* entity;
			

			entities.add(entity);
			entity->Start();
		}
	}
}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(PLAYER);
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != NULL; iterator = iterator->next)
	{
		if (iterator->data->collider == c1)
		{
			iterator->data->OnCollision(c1, c2);
			break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	if (player != nullptr)
	{
		player->Load(data);
	}


	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	if (player != nullptr)
	{
		player->Save(data);
	}



	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next)
	{
	}

	return true;
}