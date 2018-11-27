#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;


private:

	_Mix_Music * music;
	p2List<Mix_Chunk*>	fx;

public:
	uint		fade_time;
	uint		volume_music;
	uint		max_volume;
	uint		volume_sfx;
	uint 		setdown_volume_fx = false;
	uint 		setup_volume_fx = false;

	bool mute;
};

#endif // __j1AUDIO_H__