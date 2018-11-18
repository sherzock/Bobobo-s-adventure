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
#include "j1FlyingEnemy.h"
#include "j1WalkingEnemy.h"
#include "Brofiler/Brofiler.h"



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

	BROFILER_CATEGORY("EntityManager PreUpdate", Profiler::Color::MediumVioletRed)
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

bool j1EntityManager::Awake(pugi::xml_node& config) {



	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Awake(config);
	}

	return true;

}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::Tomato)

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->Update(dt);
		}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::MediumVioletRed)

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->PostUpdate();
		}

	return true;
}

bool j1EntityManager::CleanUp()
{

	bool ret = true;

	p2List_item<j1Entity*>* item;
	for (item = entities.end; item != NULL && ret == true; item = item->prev)
	{
		ret = item->data->CleanUp();
	}

	entities.clear();

	return ret;
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
	case FLYINGENEMY:
		ret = new j1FlyingEnemy(x, y, type);
		if (ret != nullptr) entities.add(ret);
		break;
	case WALKINGENEMY:
		ret = new j1WalkingEnemy(x, y, type);
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
			
			 if (queue[i].type == FLYINGENEMY) {
				entity = new j1FlyingEnemy(info.position.x, info.position.y, info.type);
			}
			 if (queue[i].type == WALKINGENEMY) {
				 entity = new j1WalkingEnemy(info.position.x, info.position.y, info.type);
			 }
			
			entities.add(entity);
			entity->Start();
			break;
		}
	}
}


void j1EntityManager::DestroyEntities()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		queue[i].type = entitytypes::NO_TYPE;
	}

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next) {

		iterator->data->CleanUp();
		int num = entities.find(iterator->data);
		RELEASE(entities.At(num)->data);
		entities.del(entities.At(num));
	}
}

void j1EntityManager::DestroyEnemies() {

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		queue[i].type = entitytypes::NO_TYPE;
	}

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next) {
		if (iterator->data->type != entitytypes::PLAYER) {
			iterator->data->CleanUp();
			int num = entities.find(iterator->data);
			RELEASE(entities.At(num)->data);
			entities.del(entities.At(num));
		}
		
	}


}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(PLAYER);
}

void j1EntityManager::OnCollision(Collider* col_1, Collider* col_2) 
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != NULL; iterator = iterator->next)
	{
		if (iterator->data->collider == col_1)
		{
			iterator->data->OnCollision(col_1, col_2);
			break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL )
	{
		entity->data->Load(data);

		entity = entity->next;
	}



	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	
	p2List_item <j1Entity*> *entity = entities.start;

	while (entity != NULL)
	{
		entity->data->Save(data);

		entity = entity->next;
	}

	return true;
}