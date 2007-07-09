struct tile
{
	int tile_number;
};

struct tile_sprite
{
	Bitmap bmp;
	int ID;

	tile_sprite(int ID) : ID(0)
	{
		printf("ID: %i\n",ID);
	}
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
			tile_sprite temp(ID);
			
			tiles.push_back(temp);
			tiles.back().bmp.Load(buffer);

			printf("associated %i with %s\n",ID,buffer.c_str());
			buffer = "";
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
			std::getline(f,buffer,',');
			temp.x = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.y = toNumber(buffer);
			std::getline(f,buffer,',');
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
			std::getline(f,buffer,',');
			temp.x = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.y = toNumber(buffer);
			std::getline(f,buffer,',');
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

			current.ents.push_back(temp);
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
			std::getline(f,buffer,',');
			temp.x = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.y = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.r = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.g = toNumber(buffer);
			std::getline(f,buffer,',');
			temp.b = toNumber(buffer);
			std::getline(f,buffer);
			temp.radius = toNumber(buffer);

			current.lights.push_back(temp);
		}
	}
	void start_mapXY(int &x, int &y, int cx, int cy)
	{
		for(int Y = 0; Y < current.height; ++Y)
		{
			if(cy >= (Y*32) && cy <= (Y*32)+32)
			{
				for(int X = 0; X < current.width; ++X)
				{
					if(cx >= (X*32) && cx <= (X*32)+32)
					{
						x = X;
						y = Y;
						return;
					}
				}
			}
		}
	}
	void draw_tile(SDL_Surface *dest, int ID, int x, int y)
	{
		for(unsigned int i = 0; i < tiles.size(); ++i)
		{
			if(tiles[i].ID == ID)
			{
				tiles[i].bmp.Draw(dest,x,y);
				return;
			}
		}
	}
	void draw_safe(SDL_Surface *dest, int cx, int cy)
	{
		for(unsigned int i = 0; i < current.safe.size(); ++i)
		{
			area &a = current.safe[i];

			for(int y = a.y; y < a.y+a.h; ++y)
			{
				for(int x = a.x; x < a.x+a.w; ++x)
				{
					if(x == a.x || y == a.y || x == (a.x+a.w)-1 || y == (a.y+a.h)-1)
					{
						Pixel(dest,x-cx,y-cy,Colour(0,255,0,255));
					}
					else Pixel(dest,x-cx,y-cy,Colour(100,255,100,128));
				}
			}
		}
	}
	void draw_block(SDL_Surface *dest, int cx, int cy)
	{
		for(unsigned int i = 0; i < current.blocking.size(); ++i)
		{
			area &a = current.blocking[i];

			for(int y = a.y; y < a.y+a.h; ++y)
			{
				for(int x = a.x; x < a.x+a.w; ++x)
				{
					if(x == a.x || y == a.y || x == (a.x+a.w)-1 || y == (a.y+a.h)-1)
					{
						Pixel(dest,x-cx,y-cy,Colour(255,0,255,255));
					}
					else Pixel(dest,x-cx,y-cy,Colour(255,100,255,128));
				}
			}
		}
	}
	void draw_lights(SDL_Surface *dest)
	{
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

		tiles.clear();

		load_wh(f);
		printf("width:%i,height:%i\n",current.width,current.height);
		load_palette(f);
		printf("tile palette size:%i\n",tiles.size());
		load_tiles(f);
		printf("tile count:%i\n",current.tiles.size());
		load_safe(f);
		printf("safe zones:%i\n",current.safe.size());
		load_block(f);
		printf("block zones:%i\n",current.blocking.size());
		load_ents(f);
		printf("entities:%i\n",current.ents.size());
		load_lights(f);
		printf("lights:%i\n",current.lights.size());

		f.close();
	}
	void Draw(SDL_Surface *dest, int view_x, int view_y)
	{
		int start_x, start_y;
		start_mapXY(start_x,start_y,view_x,view_y);

		for(int y = 0; y < 19; ++y)
		{
			if(y+start_y >= current.height) break;

			for(int x = 0; x < 25; ++x)
			{
				if(x+start_x >= current.width) break;

				int index = (y+start_y) * current.width + (x+start_x);
				int ID = current.tiles[index].tile_number;
				draw_tile(dest,ID,x*32-view_x,y*32-view_y);
			}
		}

		draw_safe(dest,view_x,view_y);
		draw_block(dest,view_x,view_y);
	}
};
