struct tile
{
	int tile_number;
};

struct tile_sprite
{
	int ID;
	Bitmap bmp;
};

struct area
{
	int x,y,w,h;
};

struct light
{
	int x,y, r,g,b, radius;
};

struct entity
{
	int x,y, ID;
};

int toNumber(const std::string &str)
{
	std::stringstream convert(str);
	int ret;
	convert >> ret;
	return ret;
}

struct map
{
	std::vector<area> blocking;
	std::vector<light> lights;
	std::vector<entity> ents;
	std::vector<tile> tiles;
	std::vector<area> safe;
	int width, height;
};

class MapSystem
{
	std::vector<tile_sprite> tiles;
	map current;
public:
};
