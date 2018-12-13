#ifndef __j1MAINMENUSCENE_H__
#define __j1MAINMENUSCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class WindowUI;
class ButtonUI;
class LabelUI;
class ImageUI;

class j1MainMenuScene : public j1Module
{
public:

	j1MainMenuScene();

	// Destructor
	virtual ~j1MainMenuScene();

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

	bool OnEventChange(j1UIItems* elem, Event event);
	void change_scenes0();

private:
	SDL_Texture * debug_tex;
	ImageUI* MainMenubg = nullptr;
	ImageUI* MainMenuTitle = nullptr;
	ButtonUI* playbutt = nullptr;
	ButtonUI* continuebutt = nullptr;
	ButtonUI* settingsbutt = nullptr;
	ButtonUI* creditsbutt = nullptr;
	ButtonUI* exitbutt = nullptr;

};

#endif // __j1MAINMENUSCENE_H__