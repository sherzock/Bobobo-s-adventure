#ifndef __j1UIITEMS_H__
#define __j1UIITEMS_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"
#include "p2Point.h"

struct SDL_Texture;

enum Position { NO_POSITIONING, POSITION_LEFT = 1, POSITION_CENTER, POSITION_RIGHT };
enum Input { IDLE = 0, HOVER, PRESSED_R, UP_R, PRESSED_L, UP_L, DISABLED };
enum Event { MOUSE_INSIDE = 1, MOUSE_OUTSIDE, LEFT_CLICK, LEFT_CLICK_UP, RIGHT_CLICK, RIGHT_CLICK_UP };
enum Type { CHECK = 1, IMAGE, BUTTON, INPUTBOX, LABEL, WINDOW };
enum Fonts { ALLER_LT, SLKSCR};

class j1UIItems : j1Module
{
public:
	j1UIItems(Type guitype, Position ali, j1Module* auxmodule = nullptr);
	virtual ~j1UIItems();


	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual bool Start();
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt);
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp();

	void UpdatePosition();
	virtual void ShowDebug();
	virtual void Move(iPoint movement) {};
	virtual void MouseInput(Input state) {};

	SDL_Texture* TexLoad(p2SString path);
	SDL_Texture* texture = nullptr;

	Type guitype;
	Event event = Event::MOUSE_OUTSIDE;
	Event event_before = Event::MOUSE_OUTSIDE;
	Input inp = Input::IDLE;
	Position ali;

	SDL_Rect rect;
	iPoint position;
	iPoint movement;
	j1Module* auxmodule;

	bool show = true;
	bool deleting = false;
	bool kinetic = true;

private:

	bool Blit() const { return true; }
	bool debug = false;

protected:
	iPoint OnClick = { 0,0 };
};

#endif // __j1UIITEMS_H__
