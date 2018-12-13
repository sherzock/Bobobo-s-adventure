#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1UIItems.h"
#include "LabelUI.h"
#include "ImageUI.h"
#include "ButtonUI.h"
#include "WindowUI.h"


class j1Gui : public j1Module
{
public:

	j1Gui();
	virtual ~j1Gui();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	SDL_Texture* GetAtlas() const;

	ImageUI* CreateImage(Position ali, char* path, SDL_Rect texture = { 0,0,0,0 }, iPoint movement = { 0,0 }, j1Module* auxmodule = nullptr);
	ButtonUI* CreateButton(Position ali, p2SString text, iPoint movement = { 0,0 }, j1Module* auxmodule = nullptr);
	LabelUI* CreateText(Position ali, p2SString text, iPoint movement = { 0,0 }, Fonts = ALLER_LT, SDL_Color color = { 255,255,255,255 }, j1Module* auxmodule = nullptr);
	WindowUI* WindowCreate(Position ali, uint num_buttons, p2SString title, iPoint movement = { 0,0 }, j1Module* auxmodule = nullptr);

private:


	j1UIItems * CreateItem(Type guitype, Position ali = Position::NO_POSITIONING);

	void ItemEventUpdate() const;
	void ItemDestory(j1UIItems* item);

	bool debug = false;
	bool ItemEventManager();

	p2List<j1UIItems*> items;

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__