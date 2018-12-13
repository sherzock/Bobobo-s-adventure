#ifndef __LABELUI_H__
#define __LABELUI_H__

#include "j1UIItems.h"
#include "SDL_TTF\include\SDL_ttf.h"

class LabelUI : public j1UIItems
{
public:

	LabelUI(Position ali = NO_POSITIONING);
	virtual ~LabelUI();

	bool Start() override;
	bool CleanUp() override;
	bool Update(float dt) override;

	void CreateText(p2SString txt, SDL_Color color, Fonts font);
	void UpdateText(p2SString newtext);
	void Move(iPoint displace);

private:
	p2SString text;
	SDL_Color text_color;
	_TTF_Font* text_font = nullptr;

};

#endif // __LABELUI_H__