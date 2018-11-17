#ifndef __P2ANIMATIONS_H__
#define __P2ANIMATIONS_H__

#include "p2SString.h"
#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 100

class Animations
{
public:
	
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

	pugi::xml_document animations;
private:
	
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect){
		frames[last_frame++] = rect;
	}


	SDL_Rect& GetCurrentFrame(){
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	void LoadAnimations(p2SString name){
		pugi::xml_parse_result result = animations.load_file("animations.xml");
		if (result != NULL){
			pugi::xml_node animation_name = animations.child("animations").child("player").child(name.GetString());
			loop = animation_name.attribute("loop").as_bool();
			speed = animation_name.attribute("speed").as_float();
			for (pugi::xml_node animation = animation_name.child("animation"); animation; animation = animation.next_sibling("animation"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
			}
		}
	}


	void LoadAnimationsEnemies(p2SString name) {
		pugi::xml_parse_result result = animations.load_file("animations.xml");
		if (result != NULL) {
			pugi::xml_node animation_name = animations.child("animations").child("enemies").child(name.GetString());
			loop = animation_name.attribute("loop").as_bool();
			speed = animation_name.attribute("speed").as_float();
			for (pugi::xml_node animation = animation_name.child("animation"); animation; animation = animation.next_sibling("animation"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
			}
		}
	}


	bool Finished(){
		if (loops > 0) {
			loops = 0;
			return true;
		}
		else
			return false;
	}

	void Reset(){
		current_frame = 0;
	}
};

#endif // __P2ANIMATION_H__
