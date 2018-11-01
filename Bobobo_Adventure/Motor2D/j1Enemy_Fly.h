#ifndef __J1ENEMY_FLY_H_
#define __J1ENEMY_FLY_H__

#include "j1Enemy.h"


class j1Enemy_Fly : public j1Enemy
{
private:
	float wave = -1.0f;
	bool going_right = true;
	Animations tree;
	Uint32 shoot_delay = 0;

public:

	iPoint original_position;
	j1PathFinding* path = new j1PathFinding();
	j1Enemy_Fly(int x, int y);

	void Move();
};

#endif // __ENEMY_BIG_EYE_H__
