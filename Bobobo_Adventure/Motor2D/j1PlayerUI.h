#ifndef __j1PLAYERUI_H__
#define __j1PLAYERUI_H__

#include "j1Player.h"
#include "j1Timer.h"

class ImageUI;
class LabelUI;
class WindowUI;
class ButtonUI;

class j1PlayerUI
{
public:
	j1PlayerUI();
	~j1PlayerUI();


	bool Start();
	bool Update(float dt);
	bool CleanUp();
	bool OnEventChange(j1UIItems* item, Event evnt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	

private:
	j1Timer		timer;

	ImageUI* heart = nullptr;
	ImageUI* Coin = nullptr;
	LabelUI* LifeCounter = nullptr;
	LabelUI* TimeCounter = nullptr;
	LabelUI* CoinCounter = nullptr;
	LabelUI* PointCounter = nullptr;
	WindowUI* window = nullptr;


};

#endif //__j1PLAYERUI_H__