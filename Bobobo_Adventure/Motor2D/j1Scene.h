#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void change_scenes1();
	void change_scenesmainmenu();
	void AddAllEnemies();

public:
	bool playeron = false;
	bool deadrestart = false;
	bool WantToLoad = false;

	j1Timer timer;
	uint sctime;
	
private:
	SDL_Texture * debug_tex;
};

#endif // __j1SCENE_H__