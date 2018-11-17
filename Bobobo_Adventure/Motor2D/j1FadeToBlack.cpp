#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Window.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Brofiler/Brofiler.h"

j1FadeToBlack::j1FadeToBlack()
{
	pugi::xml_document Fade;
	pugi::xml_parse_result result = Fade.load_file("config.xml");

	int w = Fade.child("config").child("window").child("resolution").attribute("width").as_int();
	int h = Fade.child("config").child("window").child("resolution").attribute("height").as_int();
	int s = Fade.child("config").child("window").child("resolution").attribute("scale").as_int();

	screen = { 0, 0, w * s, h * s };
	name.create("fade");
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	BROFILER_CATEGORY("FadeToBlack Update", Profiler::Color::Tomato)

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool j1FadeToBlack::Iteration()
{
	return current_step == fade_step::fade_to_black;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time){
	bool ret = false;
	ModuleOff = module_off;
	ModuleOn = module_on;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_from_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 5000.0f);
		ret = true;
	}
	return ret;
}
