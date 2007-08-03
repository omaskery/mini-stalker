struct area
{
	int x,y,w,h;
};

struct areas
{
	std::vector<area> a;
	std::string name;
	
	bool collision(int x, int y, int w, int h)
	{
		for(unsigned int i = 0; i < a.size(); ++i)
		{
			area &ar = a[i];

			if(x >= ar.x && x <= ar.x+ar.w || x+w >= ar.x && x+w <= ar.x+ar.w)
			{
				if(y >= ar.y && y <= ar.y+ar.h || y+h >= ar.y && y+h <= ar.y+ar.h)
				{
					return true;
				}
			}
		}

		return false;
	}
};

struct tile
{
	SDL_Surface *sprite;
	std::string file;
	int ID;
};

struct position
{
	int x,y,dataA,dataB;
};

struct map
{
	std::vector<position> pos;
	std::vector<tile> sprites;
	std::vector<areas> zones;
	std::vector<int> tiles;
	int w,h;
};
