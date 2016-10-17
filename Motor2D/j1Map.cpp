#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

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
	draw_margin = config.child("draw_margin").attribute("value").as_int();

	offset = 0;

	fit_square.x = fit_square.y = 0;
	fit_square.w = 1024;
	fit_square.h = 768;

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 5(old): Prepare the loop to draw all tilesets + Blit
	p2List_item<MapLayer*>* layer = data.layers.start; // for now we just use the first layer and tileset

	while (layer != nullptr) {
		p2List_item<Atribute*>* attr = layer->data->properties.start;
		while (attr != nullptr){
			if (attr->data->name == "Draw") break;
			attr = attr->next;
		}
		if (attr != nullptr && attr->data->as_bool()){
			p2List_item<TileSet*>* tileset = data.tilesets.start;
			while (tileset != nullptr) {
				int x_ini, x_end;
				TilesToDraw_x(x_ini, x_end, layer->data);
				int count = 0;
				for (int x = x_ini; x < x_end; x++) {
					int y_ini, y_end;
					TilesToDraw_y(count, x, x_end, y_ini, y_end, layer->data);

					for (int y = y_ini; y < y_end; y++) {
						iPoint world_pos = MapToWorld(x, y);
						int id = layer->data->Get(x, y);
						SDL_Rect tile_rect = tileset->data->GetTileRect(id);

						App->render->Blit(tileset->data->texture, world_pos.x, world_pos.y, &tile_rect);
					}
					count++;
				}
				tileset = tileset->next;
			}
		}
		layer = layer->next;
	}

	// TODO 10(old): Complete the draw function
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);
	// TODO 8(old): Create a method that translates x,y coordinates from map positions to world positions
	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x*data.tile_width;
		ret.y = y*data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = (x - y)*(data.tile_width * 0.5f);
		ret.y = (x + y)*(data.tile_height * 0.5f);
		break;
	}
	// TODO 1: Add isometric map to world coordinates
	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);
	// TODO 2: Add orthographic world to map coordinates
	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = (float)((float)x / (float)data.tile_width) + (float)((float)y / (float)data.tile_height);
		ret.y = (float)((float)y / (float)data.tile_height) - (float)((float)x / (float)data.tile_width);
		break;
	}
	// TODO 3: Add the case for isometric maps to WorldToMap
	return ret;
}

void j1Map::DrawFitSquare()
{
	fit_square.x = -App->render->camera.x+offset;
	fit_square.y = -App->render->camera.y+offset;
	App->render->DrawQuad(fit_square, 255, 255, 255, 255, false);
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = {0, 0, 0, 0};
	// TODO 7(old): Create a method that receives a tile id and returns it's Rect
	int real_id = id - firstgid;

	if (real_id >= 0) {

		int y = real_id / num_tiles_width;
		int x = real_id % num_tiles_width;

		if (y < num_tiles_height || x < num_tiles_width) {

			rect.x = margin + (x*tile_width) + (spacing*x);
			rect.y = margin + (y*tile_height) + (spacing*y);
			rect.h = tile_height;
			rect.w = tile_width;
		}
	}
	return rect;
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
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

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
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
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

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
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

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	pugi::xml_node properties = node.child("properties");

	for (pugi::xml_node prop = properties.child("property"); prop; prop = prop.next_sibling("property")) {
		Atribute tmp;
		tmp.name = prop.attribute("name").value();
		p2SString type = prop.attribute("type").value();
		if (type == "bool")
			tmp.type = BOOL_;
		else if (type == "int")
			tmp.type = INT_;
		else if (type == "float")
			tmp.type = FLOAT_;
		switch (tmp.type)
		{
		case BOOL_: {
			p2SString val = prop.attribute("value").value();
			tmp.value.bool_value = (val == "true") ? true : false;
			break; 
		}
		case INT_:
			tmp.value.int_value = prop.attribute("value").as_int();
			break;
		case FLOAT_:
			tmp.value.float_value = prop.attribute("value").as_float();
			break;
		}
		layer->properties.add(new Atribute(tmp));
	}

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		if (layer_data.attribute("encoding")) { 
			p2SString encoding = layer_data.attribute("encoding").value();
			if (encoding == "csv") {
				p2SString string_data = layer_data.child_value();

				p2List<p2SString> tiles;
				string_data.Tokenize(',', &tiles);

				p2List_item<p2SString>* tile = tiles.start;
				uint i = 0;
				while (tile != nullptr) {
					layer->data[i++] = atoi(tile->data.GetString());
					tile = tile->next;
				}
			}
			else if (encoding == "base64") { //not supported yet
				LOG("Base64 encoding Map NOT SUPPORTED......");
			}
		}
		else {
			int i = 0;
			for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
			{
				layer->data[i++] = tile.attribute("gid").as_int(0);
			}
		}
	}

	return ret;
}

void j1Map::TilesToDraw_x(int & x_ini, int & x_end, MapLayer* layer) const
{
	
	iPoint ini = WorldToMap(fit_square.x, fit_square.y);
	iPoint end = WorldToMap(fit_square.x + fit_square.w, fit_square.y + fit_square.h);
	switch (data.type)
	{
	case MAPTYPE_UNKNOWN:
		break;
	case MAPTYPE_ORTHOGONAL:
		x_ini = (ini.x - draw_margin > 0) ? (ini.x - draw_margin) : 0;
		x_end = (end.x + draw_margin < layer->width) ? (end.x + draw_margin) : layer->width;
		break;
	case MAPTYPE_ISOMETRIC:
		x_ini = (ini.x - draw_margin > 0) ? (ini.x - draw_margin) : 0;
		x_end = (end.x + draw_margin < layer->width) ? (end.x + draw_margin) : layer->width;
		break;
	case MAPTYPE_STAGGERED:
		break;
	default:
		break;
	}
	
}

void j1Map::TilesToDraw_y(int count, int x, int x_end, int & y_ini, int & y_end, MapLayer * layer) const
{
	iPoint top_right = WorldToMap(fit_square.x + fit_square.w, fit_square.y);
	iPoint top_left = WorldToMap(fit_square.x, fit_square.y);
	iPoint bot_right = WorldToMap(fit_square.x + fit_square.w, fit_square.y + fit_square.h);
	iPoint bot_left = WorldToMap(fit_square.x, fit_square.y + fit_square.h);
	switch (data.type)
	{
	case MAPTYPE_UNKNOWN:
		break;
	case MAPTYPE_ORTHOGONAL:
		y_ini = (top_left.y - draw_margin > 0) ? (top_left.y - draw_margin) : 0;
		y_end = (bot_right.y + draw_margin < layer->height) ? bot_right.y + draw_margin : layer->height;
		break;
	case MAPTYPE_ISOMETRIC: {
		float screen_prop = (float)fit_square.w / (float)fit_square.h;
		int diag1 = bot_left.x, diag2 = top_right.x;
		if (x <= diag1) {
			y_ini = top_left.y - (count + 1);
			y_end = top_left.y + count+2;
			if (top_left.x <= 0) {
				y_ini += (top_left.x-3);
				y_end -= (top_left.x-3);
			}
		}
		else if (x > diag1 && x < diag2) {
			int diff = x - diag1;
			y_ini = top_left.y - count - diff;
			y_end = top_left.y + diff;
		}
		else {
			int rev_count = bot_right.x - x;
			y_ini = bot_right.y - rev_count;
			y_end = bot_right.y + (rev_count + 1);
		}
		if (y_ini < 0) y_ini = 0;
		if (y_end > layer->height) y_end = layer->height;
		break; 
	}
	case MAPTYPE_STAGGERED:
		break;
	default:
		break;
	}
}


