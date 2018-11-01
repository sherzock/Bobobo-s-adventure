#ifndef __J1ENEMY_H__
#define __J1ENEMY_H__

#include "p2Point.h"
#include "p2Animations.h"
#include "j1PathFinding.h"

struct SDL_Texture;
struct Collider;
enum enemytypes {
	NO_TYPE,


};

class j1Enemy
{
protected:
	Animations * animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;
	enemytypes type = NO_TYPE;
	float flip = false;
	int live = 1;
	bool hit = false;
	//Uint32 start_time;
	uint score = 0;
public:
	j1Enemy(int x, int y);
	virtual ~j1Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites, float flip = 0.0f);
	virtual void OnCollision(Collider* collider);
};


#endif // __ENEMY_H__