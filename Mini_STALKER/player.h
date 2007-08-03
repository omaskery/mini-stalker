struct player
{
	
	SDL_Rect rect;
	float speed;
	float x,y;
	int w,h;

	unsigned int anim_delay;

	inventory inv;

	float mx,my; //used for button movement

	player() : mx(0),my(0),anim_delay(clock())
	{
		
		if(!sprite) return;

		//SDL_SetColorKey(sprite,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(sprite->format,255,0,255));
		//w = sprite->w;
		//h = sprite->h;
		//rect.w = w;
		//rect.h = h;
	}

	void next(int *nx, int *ny)
	{
		*nx = (int) (x + mx);
		*ny = (int) (y + my);
	}
	void move()
	{
		x += mx;
		y += my;
		rect.x = (Sint16)x - cam.x;
		rect.y = (Sint16)y - cam.y;
	}
	void draw(SDL_Surface *s)
	{
	}
	void handle(SDL_Event &e)
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			{
				switch(e.key.keysym.sym)
				{
				case SDLK_RIGHT:
					{
						mx = speed;
					} break;
				case SDLK_LEFT:
					{
						mx = -speed;
					} break;
				case SDLK_UP:
					{
						my = -speed;
					} break;
				case SDLK_DOWN:
					{
						my = speed;
					} break;
				}
			} break;
		case SDL_KEYUP:
			{
				switch(e.key.keysym.sym)
				{
				case SDLK_RIGHT:
				case SDLK_LEFT:
					{
						mx = 0;
					} break;
				case SDLK_UP:
				case SDLK_DOWN:
					{
						my = 0;
					} break;
				} break;
			}
		}
	}
};
