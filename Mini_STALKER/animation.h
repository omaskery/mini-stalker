class animation
{
	std::vector<SDL_Surface *> frames;
	unsigned int timer, delay;
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
			if(current >= frames.size()) current = 0;
		}

		SDL_BlitSurface(frames[current],NULL,dest,&my_rect);
	}
};
