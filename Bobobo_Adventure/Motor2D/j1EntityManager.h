#ifndef __J1ENEMIES_H__
#define __J1ENEMIES_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "j1Collisions.h"

#define MAX_ENEMIES 50

class j1Entity;
class j1Player;


struct SDL_Texture;

enum entitytypes
{
	NO_TYPE,
	PLAYER,
	FLYINGENEMY

};

struct EnemyInfo
{
	entitytypes type = entitytypes::NO_TYPE;
	iPoint position;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Awake(pugi::xml_node& config);
	bool PostUpdate();
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(entitytypes type, int x = 0, int y = 0);

	void OnCollision(Collider* col_1, Collider* col_2);
	void CreatePlayer();
	void AddEnemy(int x, int y, entitytypes type);
	void CreateEnemy(const EnemyInfo& info);
	void DestroyEntities();

public:

	p2List<j1Entity*>	entities;

	j1Player*			player = nullptr;
	

	SDL_Texture*		playerSprite = nullptr;
	

private:

	EnemyInfo			queue[MAX_ENEMIES];

};

#endif // __J1ENEMIES_H__