#ifndef __j1SCENE2_H__
#define __j1SCENE2_H__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;

class WindowUI;
class ButtonUI;
class LabelUI;
class ImageUI;
class SliderUI;

class j1Scene2 : public j1Module
{
public:

	j1Scene2();

	// Destructor
	virtual ~j1Scene2();

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

	void change_scenes2();
	void change_scenesmainmenu();
	void AddAllEnemies2();
	void createescwindow();
	void destroyescwindow();

	bool deadrestart = false;
	bool WantToLoad = false;
	bool iswindowon = false;

	j1Timer timer;
	uint sc2time;
	WindowUI* escwindow;
	ButtonUI* escresume;
	ButtonUI* escmainmenu;

private:
	bool playeron = false;

};

#endif // __j1SCENE2_H__