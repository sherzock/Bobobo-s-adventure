#ifndef _j1Coin_H__
#define __j1Coin_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2Animations.h"
#include "p2DynArray.h"
#include "j1Entity.h"

class j1Coin : public j1Entity
{
public:


	j1Coin(int x, int y, entitytypes type);

	virtual ~j1Coin();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);
	

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



public:

	Animations Coin_anim;
	fPoint initPos;
	iPoint colliderSize;
	
};

#endif // _j1FlyingEnemy_H__
