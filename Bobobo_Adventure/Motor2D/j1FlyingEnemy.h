#ifndef _j1FlyingEnemy_H__
#define __j1FlyingEnemy_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2Animations.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"

class j1FlyingEnemy : public j1Entity
{
public:
	void Pathing(p2DynArray<iPoint>& path, float dt);

	j1FlyingEnemy(int x, int y, entitytypes type);

	virtual ~j1FlyingEnemy();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void fly(p2DynArray<iPoint>& path, float dt);
	void OnCollision(Collider* col_1, Collider* col_2);
	void Sleeping();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



public:

	p2DynArray<iPoint>* path;
	bool path_created = false;
	Animations flying;
	fPoint initPos;
	iPoint colliderSize;
	int range = 1000;
	Movement direction;
};

#endif // _j1FlyingEnemy_H__