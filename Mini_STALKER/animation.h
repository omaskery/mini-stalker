class animation
{
	std::vector<SDL_Surface *> frames;
	unsigned int timer, delay;
	int *bind_x, *bind_y;
	int x,y,current;

	SDL_Rect my_rect;
public:
	animation(const std::string &file) : timer(clock())
	{
		std::ifstream f(file.c_str());
		if(!f) return;

		int count;
		f >> delay >> count;
		f.ignore();

		for(int i = 0; i < count; ++i)
		{
			std::string buffer;
			std::getline(f,buffer);
			SDL_Surface *temp = SDL_LoadBMP(buffer.c_str());
			if(temp)
			{
				SDL_SetColorKey(temp,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(temp->format,255,0,255));
				frames.push_back(temp);
			}
			else
			{
				printf("unable to load animation sprite %s for animation %s.\n",buffer.c_str(),file.c_str());
			}
		}

		f.close();
	}
	void draw(SDL_Surface *dest)
	{
		if(clock()-timer >= delay)
		{
			++current;
			timer = clock();
			if(current >= frames.size()) current = 0;
		}

		if(bind_x) my_rect.x = *bind_x;
		if(bind_y) my_rect.y = *bind_y;

		SDL_BlitSurface(frames[current],NULL,dest,&my_rect);
	}
	void setPosition(int X, int Y)
	{
		x = X;
		y = Y;
		my_rect.x = x;
		my_rect.y = y;
	}
	void setPosition(int *X, int *Y)
	{
		bind_x = X;
		bind_y = Y;

		my_rect.x = *X;
		my_rect.y = *Y;

		if(!X) my_rect.x = x;
		if(!Y) my_rect.y = Y;
	}
};
