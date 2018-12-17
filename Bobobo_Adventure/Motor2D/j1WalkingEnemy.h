#ifndef _j1WalkingEnemy_H__
#define __j1WalkingEnemy_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2Animations.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"

class j1WalkingEnemy : public j1Entity
{
public:

	j1WalkingEnemy(int x, int y, entitytypes type);

	virtual ~j1WalkingEnemy();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void walk(p2DynArray<iPoint>& path, float dt);
	void OnCollision(Collider* col_1, Collider* col_2);
	void LoadXML();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



public:

	p2DynArray<iPoint>* path = nullptr;
	bool path_created = false;
	Animations idle;
	Animations walking;
	fPoint initPos;
	iPoint colliderSize;
	int range = 1000;
	Movement direction;
};

#endif // _j1WalkingEnemy_H__