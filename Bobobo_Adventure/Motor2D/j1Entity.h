#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__


#include "j1Module.h"
#include "p2Point.h"
#include "p2Animations.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"


#define GRAVITY 7

struct Collider;
struct SDL_Texture;
struct Animation;
enum entitytypes;

class j1Entity{
public:
	j1Entity(int x, int y, entitytypes type);

	virtual bool Start() {
		return true;
	};
	virtual bool Update(float dt) {
		return true;
	};
	virtual bool PostUpdate() {
		return true;
	};
	virtual bool PreUpdate() {
		return true;
	};
	virtual bool CleanUp() {
		return true;
	};

public:

	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	bool goingright = false;
	
	fPoint position;
	float speed;
	iPoint initial_pos;
	Animations* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprites = nullptr;
	SDL_Texture* win_sprites = nullptr;
	bool paused = false;
	entitytypes type;

	virtual void Draw();
	virtual void OnCollision(Collider* c1, Collider* c2) {};
	

};

#endif // __J1ENTITY_H__