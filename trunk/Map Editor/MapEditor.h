struct tile
{
	int tile_number;
};

struct tile_sprite
{
	Bitmap bmp;
	int ID;

	tile_sprite(const std::string &str, int ID) : bmp(str), ID(ID)
	{
	}
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
	void load_wh(std::ifstream &f)
	{
		std::string buffer;
		std::getline(f,buffer,',');
		current.width = toNumber(buffer);
		std::getline(f,buffer);
		current.height = toNumber(buffer);
	}
	void load_palette(std::ifstream &f)
	{
		std::string buffer;
		int paletteCount;
		f >> paletteCount;
		f.ignore();

		for(int i = 0; i < paletteCount; ++i)
		{
			int ID;
			std::getline(f,buffer,':');
			ID = toNumber(buffer);
			std::getline(f,buffer);
			printf("%i:%s\n",ID,buffer.c_str());
			tile_sprite temp(buffer,ID);
			
			tiles.push_back(temp);
		}
	}
	void load_tiles(std::ifstream &f)
	{
		int tileCount = current.width * current.height;

		for(int i = 0; i < tileCount; ++i)
		{
			tile temp;
			f >> temp.tile_number;
			current.tiles.push_back(temp);
		}

		f.ignore();
	}
	void load_safe(std::ifstream &f)
	{
		std::string buffer;

		int safeZones;
		f >> safeZones;
		f.ignore();

		for(int i = 0; i < safeZones; ++i)
		{
			area temp;
			std::getline(f,buffer);
			temp.x = toNumber(buffer);
			std::getline(f,buffer);
			temp.y = toNumber(buffer);
			std::getline(f,buffer);
			temp.w = toNumber(buffer);
			std::getline(f,buffer);
			temp.h = toNumber(buffer);

			current.safe.push_back(temp);
		}
	}
	void load_block(std::ifstream &f)
	{
		std::string buffer;

		int bbZones;
		f >> bbZones;
		f.ignore();

		for(int i = 0; i < bbZones; ++i)
		{
			area temp;
			std::getline(f,buffer);
			temp.x = toNumber(buffer);
			std::getline(f,buffer);
			temp.y = toNumber(buffer);
			std::getline(f,buffer);
			temp.w = toNumber(buffer);
			std::getline(f,buffer);
			temp.h = toNumber(buffer);

			current.blocking.push_back(temp);
		}
	}
	void load_ents(std::ifstream &f)
	{
		std::string buffer;

		int entities;
		f >> entities;
		f.ignore();
		
		for(int i = 0; i < entities; ++i)
		{
			entity temp;
			std::getline(f,buffer,':');
			temp.ID = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.x = toNumber(buffer);
			std::getline(f,buffer);
			temp.y = toNumber(buffer);
		}
	}
	void load_lights(std::ifstream &f)
	{
		std::string buffer;

		int lights;
		f >> lights;
		f.ignore();

		for(int i = 0; i < lights; ++i)
		{
			light temp;
			std::getline(f,buffer);
			temp.x = toNumber(buffer);
			std::getline(f,buffer);
			temp.y = toNumber(buffer);
			std::getline(f,buffer);
			temp.r = toNumber(buffer);
			std::getline(f,buffer);
			temp.g = toNumber(buffer);
			std::getline(f,buffer);
			temp.b = toNumber(buffer);
			std::getline(f,buffer);
			temp.radius = toNumber(buffer);

			current.lights.push_back(temp);
		}
	}
public:
	void Load(const std::string &filename)
	{
		std::ifstream f(filename.c_str());
		if(!f)
		{
			printf("Failed to open file\n");
			return;
		}

		load_wh(f);
		printf("width:%i,height:%i\n",current.width,current.height);
		load_palette(f);
		printf("tile palette size:%i",tiles.size());
		load_tiles(f);
		printf("tile count:%i",current.tiles.size());
		load_safe(f);
		printf("safe zones:%i",current.safe.size());
		load_block(f);
		printf("block zones:%i",current.blocking.size());
		load_ents(f);
		printf("entities:%i",current.ents.size());
		load_lights(f);
		printf("width: %i height: %i palette_count: %i tiles: %i safe_zones %i block_zones %i entities: %i lights: %i",current.width,current.height,tiles.size(),current.tiles.size(),current.safe.size(),current.blocking.size(),current.ents.size(),current.lights.size());

		f.close();
	}
};
