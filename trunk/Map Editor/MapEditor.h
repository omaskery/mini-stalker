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

struct map
{
	std::vector<area> blocking;
	std::vector<light> lights;
	std::vector<entity> ents;
	std::vector<tile> tiles;
	std::vector<area> safe;
	int width, height;

	map(const std::string &filename)
	{
		load(filename);
	}

private:
	void load(const std::string &filename)
	{
	}
};
