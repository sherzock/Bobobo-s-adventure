#ifndef __j1MAINMENUSCENE_H__
#define __j1MAINMENUSCENE_H__

#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

class WindowUI;
class ButtonUI;
class LabelUI;
class ImageUI;
class SliderUI;

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
	void CanLoadGame();
	void change_scenes0();
	void createsettings();
	void createcredits();
	void createmainmenu();
	bool candocontinue = false;
private:
	SDL_Texture * debug_tex = nullptr;
	ImageUI* MainMenubg = nullptr;
	ImageUI* MainMenuTitle = nullptr;
	ButtonUI* playbutt = nullptr;
	ButtonUI* continuebutt = nullptr;
	ImageUI*  coninuebuttoff = nullptr;
	ButtonUI* settingsbutt = nullptr;
	ButtonUI* creditsbutt = nullptr;
	ButtonUI* exitbutt = nullptr;
	ButtonUI* settingsback = nullptr;
	ImageUI*  volumetitle = nullptr;
	ButtonUI* creditsback = nullptr;
	ImageUI*  creditstxt = nullptr;
	SliderUI* volslider = nullptr;
	ImageUI*  volrail = nullptr;
	ButtonUI* github = nullptr;
	uint Click_fx;




};

#endif // __j1MAINMENUSCENE_H__