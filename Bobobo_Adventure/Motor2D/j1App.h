#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1MainMenuScene;
class j1Scene;
class j1Scene2;
class j1Map;
class j1Collisions;
class j1Player;
class j1FadeToBlack;
class j1PathFinding;
class j1EntityManager;
class j1Fonts;
class j1Gui;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Audio*			audio = NULL;
	j1MainMenuScene*	menuscene = NULL;
	j1Scene*			scene = NULL;
	j1Scene2*			scene2 = NULL;
	j1Map*				map = NULL;
	j1Collisions*		colls = NULL;
	j1Player*			play = NULL;
	j1FadeToBlack*		fade = NULL;
	j1PathFinding*		path = NULL;
	j1EntityManager*	enty = NULL;
	j1Fonts*			font = NULL;
	j1Gui*				gui = NULL;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	j1PerfTimer			ptimer;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint64				frame_count = 0;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint32				frame_cap;
	uint32				msForCap;
	float				dt;
	bool				capfps = true;

};

extern j1App* App; 

#endif