#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;

class WindowUI;
class ButtonUI;
class LabelUI;
class ImageUI;
class SliderUI;

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

	bool OnEventChange(j1UIItems* item, Event event);

	void change_scenes1();
	void change_scenesmainmenu();
	void AddAllEnemies();
	void createescwindow();
	void destroyescwindow();

public:
	bool playeron = false;
	bool deadrestart = false;
	bool WantToLoad = false;

	bool iswindowon = false;

	j1Timer timer;
	uint sctime;
	WindowUI* escwindow;
	ButtonUI* escresume;
	ButtonUI* escmainmenu;


private:
	SDL_Texture * debug_tex;
};

#endif // __j1SCENE_H__