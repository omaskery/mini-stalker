class dimensions : public chunk_handler
{
public:
	void load(map &m, int size, char *chunk)
	{
		if(size < 8)
		{
			printf("unable to load map dimensions\n");
			return;
		}

		m.w = *((int*) &chunk[0]);
		m.h = *((int*) &chunk[4]);

		//printf("map: %i,%i\n",m.w,m.h);
	}
	void save(map &m, std::ofstream &file)
	{
		int size = 8;
		file.write((char*)&size,4);
		file.write((char*)&m.w,4);
		file.write((char*)&m.h,4);

		//printf("wrote dimensions: %i,%i - %i\n",m.w,m.h,size);
	}
};

class tiles : public chunk_handler
{
	void load(map &m, int size, char *chunk)
	{
		if(size < m.w*m.h*4)
		{
			printf("unable to load map tiles\n");
			return;
		}

		int pos = 0;
		int data;

		while(pos < m.w*m.h*4)
		{
			data = *((int*)&chunk[pos]);
			m.tiles.push_back(data);
			//printf("tile: %i\n",data);
			pos += 4;
		}
	}
	void save(map &m, std::ofstream &file)
	{
		int size = m.w*m.h*4;
		file.write((char*)&size,4);

		//printf("tile writing: %i\n",size);

		for(int i = 0; i < m.w*m.h; ++i)
		{
			int value = m.tiles[i];
			file.write((char*)&value,4);
		}
	}
};

class areas_chunk : public chunk_handler
{
public:
	void load(map &m, int size, char *chunk)
	{
		if(size < 4)
		{
			printf("unable to load zone information - no name length information\n");
			return;
		}

		areas a;

		int name_size;
		name_size = *((int*)&chunk[0]);

		char *name = new char[name_size+1];
		memcpy(name,&chunk[4],name_size);
		name[name_size] = 0;

		a.name = name;
		delete[] name;

		//printf("name: %i, %s\n",name_size,a.name.c_str());

		int area_count;
		area_count = *(int*)&chunk[4+name_size];

		//std::cout << name_size << "/" << a.name.size() << " -> " << a.name << " -> " << area_count << "\n";

		//printf("areas: %i\n",area_count);

		if(size < 8 + name_size + area_count * (int)sizeof(area))
		{
			printf("unable to load zone information - not enough data for areas\n");
			return;
		}

		int offset = 8 + name_size;
		for(int i = 0; i < area_count; ++i)
		{
			area ar;

			ar = *((area*)&chunk[offset + i * sizeof(area)]);
			a.a.push_back(ar);

			//std::cout << "loaded area: " << ar.x << "," << ar.y << " " << ar.w << "," << ar.h << "\n";
		}

		m.zones.push_back(a);
	}
	void save(map &m, std::ofstream &file)
	{
		int size = 8 + (int)m.zones.back().name.size() + (int)m.zones.back().a.size() * (int)sizeof(area);

		file.write((char*)&size,4);

		//printf("area write size: %i\n",size);

		areas a = m.zones.back();
		m.zones.pop_back();

		int name_size = (int)a.name.size();
		file.write((char*)&name_size,4);

		file.write(a.name.c_str(),name_size);

		//printf("write name: %i - %s\n",name_size,a.name.c_str());

		unsigned int area_count = (unsigned int)a.a.size();
		file.write((char*)&area_count,4);

		//printf("write areas: %i\n",area_count);

		for(unsigned int i = 0; i < area_count; ++i)
		{
			file.write((char*)&a.a[i],sizeof(area));
		}
	}
};

class positions : public chunk_handler
{
public:
	void load(map &m, int size, char *chunk)
	{
		if(size < 4)
		{
			printf("cannot load positions - no position count data\n");
			return;
		}

		int position_count;
		position_count = *((int*)&chunk[0]);

		//printf("position count: %i\n",position_count);

		if(size < position_count*(int)sizeof(position)+4)
		{
			printf("cannot load positions - not enough data for positions\n");
			return;
		}

		int offset = 4;
		for(int i = 0; i < position_count; ++i)
		{
			position temp;
			temp = *((position*)&chunk[offset+i*sizeof(position)]);
			m.pos.push_back(temp);

			//printf("loaded position: %i,%i %i %i\n",temp.x,temp.y,temp.dataA,temp.dataB);
		}
	}
	void save(map &m, std::ofstream &file)
	{
		int size = 4 + (int)m.pos.size() * (int)sizeof(position);

		file.write((char*)&size,4);

		int p_size = (int)m.pos.size();

		file.write((char*)&p_size,4);

		for(int i = 0; i < p_size; ++i)
		{
			file.write((char*)&m.pos[i],sizeof(position));
		}
	}
};

class sprite_chunk : public chunk_handler
{
public:
	void load(map &m, int size, char *chunk)
	{
		int id, name_size, offset = 8;
		
		id = *((int*)&chunk[0]);
		name_size = *((int*)&chunk[4]);

		//printf("loading sprite: %i, %i\n",id,name_size);

		char *name = new char[name_size+1];

		memcpy(name,&chunk[offset],name_size);
		name[name_size] = 0;

		//printf("file: %s\n",name);

		tile temp;
		temp.ID = id;
		temp.file = name;
		temp.sprite = SDL_LoadBMP(name);
		if(temp.sprite)
		{
			SDL_SetColorKey(temp.sprite,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(temp.sprite->format,255,0,255));
			m.sprites.push_back(temp);
		}
		else
		{
			printf("error, unable to load %s\n",name);
			return;
		}

		delete name;
	}
	void save(map &m, std::ofstream &file)
	{
		if(!m.sprites.size())
		{
			printf("error, no sprites to save!\n");
			return;
		}

		tile t = m.sprites.back();
		m.sprites.pop_back();

		int size = (int)t.file.size() + 8;
		int f_size = (int)t.file.size();

		file.write((char*)&size,4);
		file.write((char*)&t.ID,4);
		file.write((char*)&f_size,4);
		file.write(t.file.c_str(),f_size);

		//printf("writing sprite: %i -> %s (%i) == %i\n",t.ID,t.file.c_str(),f_size,size);
	}
};
