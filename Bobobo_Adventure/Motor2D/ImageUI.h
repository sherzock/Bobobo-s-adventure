#ifndef __IMAGEUI_H__
#define __IMAGEUI_H__

#include "j1UIItems.h"

class ImageUI : public j1UIItems
{
public:

	ImageUI(Position ali = NO_POSITIONING);
	virtual ~ImageUI();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Move(iPoint Movement);
};

#endif // __IMAGEUI_H__