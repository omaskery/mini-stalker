#include "MapSystem/MapSystem_includes.h"

class MapSystem
{
	std::vector<int> load_order;
	std::map<int,chunk_handler*> handlers;
	map current;
	
	void draw_tiles(SDL_Surface *dest,int cx, int cy)
	{
		for(int y = 0; y < current.h; ++y)
		{
			if((y*32+32)-cy < 0)
			{
				continue;
			}
			if(y*32-cy >= dest->h)
			{
				break;
			}

			for(int x = 0; x < current.w; ++x)
			{
				if((x*32+32)-cx < 0)
				{
					continue;
				}
				if(x*32-cx >= dest->w)
				{
					break;
				}

				int index = x + y * current.w;
				draw_tile(dest,current.tiles[index],x*32-cx,y*32-cy);
			}
		}
	}
	void draw_tile(SDL_Surface *s,int ID, int x, int y)
	{
		for(unsigned int i = 0; i < current.sprites.size(); ++i)
		{
			if(current.sprites[i].ID != ID) continue;

			SDL_Rect dest;
			dest.x = x;
			dest.y = y;
			dest.w = 32;
			dest.h = 32;
			SDL_BlitSurface(current.sprites[i].sprite,NULL,s,&dest);
		}
	}
public:
	MapSystem()
	{
		add_handler(new dimensions(),0);
		add_handler(new tiles(),1);
		add_handler(new areas_chunk(),2);
		add_handler(new positions(),3);
		add_handler(new sprite_chunk(),4);
	}
	~MapSystem()
	{
		for(unsigned int i = 0; i < handlers.size(); ++i)
		{
			delete handlers[i];
		}
	}
	void add_handler(chunk_handler *ch,int ID)
	{
		handlers[ID] = ch;
	}
	void addSprite(const std::string &sprite_name)
	{
		tile temp;
		temp.file = sprite_name;
		temp.sprite = SDL_LoadBMP(sprite_name.c_str());
		if(!temp.sprite) return;

		temp.ID = 0;
		while(1)
		{
			bool collide = false;
			for(unsigned int i = 0; i < current.sprites.size(); ++i)
			{
				if(current.sprites[i].ID == temp.ID)
				{
					collide = true;
				}
			}

			if(!collide) break;

			++temp.ID;
		}

		current.sprites.push_back(temp);
		load_order.push_back(4);
	}
	areas *getArea(const std::string &name)
	{
		for(unsigned int i = 0; i < current.zones.size(); ++i)
		{
			if(current.zones[i].name != name) continue;

			return &current.zones[i];
		}

		return NULL;
	}
	map *getMap()
	{
		return &current;
	}
	int load(const std::string &filename)
	{
		std::ifstream f(filename.c_str(),std::ios::binary);
		if(!f) return -1;

		current = map();

		size_t s;
		f.seekg(0,std::ios::end);
		s = f.tellg();
		f.seekg(0,std::ios::beg);

		char *file = new char[s];
		f.read(file,(std::streamsize)s);
		f.close();

		unsigned int pos = 0;
		int id, size;
		while(pos < s)
		{
			id = *((int*) &file[pos]);
			size = *((int*) &file[pos+4]);
			pos = pos + 8;

			char *chunk = new char[size];
			memcpy(chunk,&file[pos],size);
			pos += size;

			//std::cout << "ID: " << id << " size: " << size << " bytes.\n";

			chunk_handler *handle = handlers[id];
			if(handle)
			{
				load_order.push_back(id);
				handle->load(current,size,chunk);
			}

			delete[] chunk;
		}

		return 0;
	}
	void save(const std::string &filename)
	{
		std::ofstream f(filename.c_str(),std::ios::binary);

		for(unsigned int i = 0; i < load_order.size(); ++i)
		{
			chunk_handler *ch = handlers[load_order[i]];

			if(!ch) continue;
			
			f.write((char*)&load_order[i],4);
			ch->save(current,f);
		}

		f.close();
	}
	void draw(SDL_Surface *destination, int view_x, int view_y)
	{
		draw_tiles(destination,view_x,view_y);
	}
};
