#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};
// ----------------------------------------------------
struct MapLayer
{
	p2SString			name;
	uint				width;
	uint				height;
	uint*				data;
	
	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}
	
	inline uint Get(int x, int y) const;
	
};

	
struct ImageLayer
{
	SDL_Rect GetParalaxRect() const;

	p2SString	name;
	int			width;
	int			height;
	SDL_Texture* texture;

	ImageLayer() {}

	~ImageLayer() {}

};

uint MapLayer::Get(int x, int y) const {
	return (y * width) + x;
}

// ----------------------------------------------------
struct TileSet
{

	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<ImageLayer*> parallax;
	
	p2List<MapLayer*> layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	
	bool LoadLayer(const pugi::xml_node& node, MapLayer* layer);
	bool Colliders_on_map(const char * filename);
	bool LoadParallax(pugi::xml_node& node, ImageLayer* image);
	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	pugi::xml_document	map_file;

private:

	
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__