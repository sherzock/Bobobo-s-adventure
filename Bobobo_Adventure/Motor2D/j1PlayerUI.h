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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	

private:
	j1Timer		timer;
	j1Timer		pausetime;

	ImageUI* heart = nullptr;
	ImageUI* Coin = nullptr;
	LabelUI* LifeCounter = nullptr;
	LabelUI* TimeCounter = nullptr;
	LabelUI* CoinCounter = nullptr;
	LabelUI* PointCounter = nullptr;


};

#endif //__j1PLAYERUI_H__