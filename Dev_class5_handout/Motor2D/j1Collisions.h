#pragma once
#ifndef __MODULECOLLISIONS_H__
#define __MODULECOLLISIONS_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

#define MAX_NUM_COLLIDERS 800

enum COLLIDER_TYPE
{
	NO_COLLIDER = 0,
	PLAYER_COLLIDER,
	GROUND_COLLIDER,
	TYPE_COLLIDER,

};

enum CollisionDirection
{
	NO_COLLISION = 0,
	FROM_ABOVE,
	FROM_BELOW,
	FROM_RIGHT,
	FROM_LEFT
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;


	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) : rect(rectangle), type(type), callback(callback) {}

	void Set_Pos(int x, int y) { rect.x = x; rect.y = y; }
	bool Check_Collision(const SDL_Rect& r) const;
	CollisionDirection Check_Direction(const SDL_Rect& r) const;
};

class j1Collisions : public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	uint Colliders_Count() const;

	bool Delete_Collider(Collider* collider);
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void Collider_to_debug();
	bool debug = false;


private:

	Collider* colliders[MAX_NUM_COLLIDERS];
	bool matrix[TYPE_COLLIDER][TYPE_COLLIDER];
};

#endif// __MODULECOLLISION_H__