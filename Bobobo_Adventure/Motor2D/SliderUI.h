#ifndef __SLIDERUI_H__
#define __SLIDERUI_H__

#include "j1UIItems.h"

class ButtonUI;
class ImageUI;

class SliderUI : public j1UIItems
{
public:

	SliderUI(Position alignment = NO_POSITIONING);
	virtual ~SliderUI();

	bool Start();
	bool CleanUp();
	bool Update(float dt);
	bool PostUpdate();

	void MouseInput(Input state);
	void ChangeEvent(j1UIItems* item, Event event);
	void Move(iPoint movement);

	void SettleTextureToSlider(p2SString path_idle, p2SString path_hover = nullptr, p2SString path_pressed = nullptr);

	void StartDrag();
	void Drag(/*iPoint point*/);
	void Enddrag();

	bool active = true;
	bool movable = false;

	iPoint pos;
	iPoint limit;

	uint lastpos = 0;

private:
	SDL_Texture * Rail = nullptr;
	SDL_Texture * None = nullptr;
	SDL_Texture* Over = nullptr;
	SDL_Texture* Pressed = nullptr;

};

#endif // __SLIDERUI_H__