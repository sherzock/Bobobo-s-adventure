#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Window.h"
#include <math.h>
#include "j1Scene.h"
#include "j1Scene2.h"


j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}


// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	
	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* layer;

	int tile_num;
	for (layer = data.layers.start; layer != nullptr; layer = layer->next)
	{
		tile_num = 0;
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->data->data[tile_num];
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);
						iPoint pos = MapToWorld(x, y);
					
						//Renders diferent distnaces for diferent speeds of layer //
							if (layer->data->name == "middle") {
								
								if (pos.x <(-(App->render->camera.x) + App->render->camera.w) && pos.x >(-(App->render->camera.x)- 650)){
								App->render->Blit(tileset->texture, pos.x, pos.y, &r, SDL_FLIP_NONE,map_file.child("map").child("layer").next_sibling("layer").child("properties").child("property").attribute("value").as_float());
								}
							}
							else if (layer->data->name == "front") {
								if (pos.x <(-(App->render->camera.x) + App->render->camera.w) && pos.x >(-(App->render->camera.x)- 1185))
								App->render->Blit(tileset->texture, pos.x, pos.y, &r, SDL_FLIP_NONE, map_file.child("map").child("layer").next_sibling("layer").next_sibling("layer").next_sibling("layer").child("properties").child("property").attribute("value").as_float());
							}
							else {
								if (pos.x <(-(App->render->camera.x) + App->render->camera.w) && pos.x >(-(App->render->camera.x) - 170)) {

								App->render->Blit(tileset->texture, pos.x, pos.y, &r, SDL_FLIP_NONE);
								}
							}
							
						
					}
				}
				tile_num++;
			}
		}
	}	

}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
	
	ret.x = (data.tile_width / 2) *x - y;
	ret.x = (data.tile_height / 2) *x - y;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;
	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}
	return set;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	
	// Remove all layers

	p2List_item<ImageLayer*>* back;
	back = data.parallax.start;

	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}

	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

SDL_Rect ImageLayer::GetParalaxRect() const
{
	SDL_Rect rect;

	rect.w = width;
	rect.h = height;

	rect.x = 0;
	rect.y = 0;


	return rect;
}
// Load new map

bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;

	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer;
		ret = LoadLayer(layer, lay);
		LOG("LOADING LAYER %s", lay->name.GetString());
		
		if(ret == true) {
		data.layers.add(lay);
		}
	}

	//Load Image info ----------------------------
	pugi::xml_node paralaxNode;
	for (paralaxNode = map_file.child("map").child("imagelayer"); paralaxNode && ret; paralaxNode = paralaxNode.next_sibling("imagelayer"))
	{
		ImageLayer* imageList = new ImageLayer();

		ret = LoadParallax(paralaxNode, imageList);

		if (ret == true)
			data.parallax.add(imageList);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		
		p2List_item<ImageLayer*>* item_imageParalax = data.parallax.start;
		while (item_imageParalax != NULL)
		{
			ImageLayer* i = item_imageParalax->data;
			LOG("Paralax image ----");
			LOG("name: %s", i->name.GetString());
			LOG("tile width: %d tile height: %d", i->width, i->height);
			item_imageParalax = item_imageParalax->next;
		}
	
	
	
	}
	
	if (App->scene->active == true) {
		Colliders_on_map("Level1map.tmx");
	}
	else if (App->scene2->active == true) {
		Colliders_on_map("Level2map.tmx");
	}


	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}


bool j1Map::LoadLayer(const pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->data = new uint[layer->width * layer->height];
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	memset(layer->data, 0, sizeof(uint)*layer->height*layer->width);

	int i = 0;

	for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i++] = tile.attribute("gid").as_uint(0);
	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}

bool j1Map::LoadParallax(pugi::xml_node& node, ImageLayer* image)
{
	bool ret = true;

	image->name = node.attribute("name").as_string();
	image->width = node.child("image").attribute("width").as_int();
	image->height = node.child("image").attribute("height").as_int();
	image->texture = App->tex->Load(PATH(folder.GetString(), node.child("image").attribute("source").as_string()));

	return ret;
}

bool j1Map::Colliders_on_map(const char * filename) {
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), filename);
	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());
	if (result == NULL)
	{
		LOG("Could not load tiled xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}
	pugi::xml_node obj;
	pugi::xml_node group;
	const char* object_name;
	for (group = map_file.child("map").child("objectgroup"); group && ret; group = group.next_sibling("objectgroup"))
	{
		object_name = group.attribute("name").as_string();
		for (obj = group.child("object"); obj && ret; obj = obj.next_sibling("object"))
		{
			App->colls->AddCollider({
				obj.attribute("x").as_int(),
				obj.attribute("y").as_int(),
				obj.attribute("width").as_int(),
				obj.attribute("height").as_int() }, 
				GROUND_COLLIDER);
		}
	}
	return ret;
}