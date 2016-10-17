#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------
enum Classes {
	INT_ = 0, FLOAT_, BOOL_
};

union pos_values {
	int int_value;
	float float_value;
	bool bool_value;
};

struct Atribute {
	p2SString name;
	pos_values value;
	Classes type;

	Atribute(Atribute& atr): name(atr.name), value(atr.value), type(atr.type){}
	Atribute(){}

	int as_int() const {
		return value.int_value;
	}

	bool as_bool() const {
		return value.bool_value;
	}

	float as_float() const {
		return value.float_value;
	}
};

struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	uint*		data;
	p2List<Atribute*>	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// TODO 6 (old): Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[x+y*width];
	}
};

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
	p2List<MapLayer*>	layers;
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

	// Coordinate translation methods
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	void DrawFitSquare();

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

	void TilesToDraw_x(int& x_ini, int& x_end, MapLayer* layer) const;
	void TilesToDraw_y(int count, int x, int x_end, int& y_ini, int& y_end, MapLayer* layer) const;

public:

	MapData data;
	SDL_Rect			fit_square;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	int					draw_margin;
	int					offset;

	
};

#endif // __j1MAP_H__