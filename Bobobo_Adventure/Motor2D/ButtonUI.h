#ifndef __ButtonUI_H__
#define __ButtonUI_H__

#include "j1UIItems.h"

class LabelUI;

class ButtonUI : public j1UIItems
{
public:

	ButtonUI(Position alignment = NO_POSITIONING);
	virtual ~ButtonUI();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void MouseInput(Input state);
	void ChangeEvent(j1UIItems* item, Event event);
	void Move(iPoint movement);

	void SettleTextureToButton(p2SString path_idle, p2SString path_hover, p2SString path_pressed = nullptr);

	bool active = true;

private:
	SDL_Texture * None = nullptr;
	SDL_Texture* Over = nullptr;
	SDL_Texture* Pressed = nullptr;

};

#endif // __GUIBUTTON_H__
