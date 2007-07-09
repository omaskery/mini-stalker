#include "includes.h"

void clear(SDL_Surface *s)
{
	int length = s->w * s->h;
	for(int i = 0; i < length; ++i)
	{
		Uint32 *bufp = (Uint32*) s->pixels + i;
		*bufp = 0;
	}
}

int main()
{
	Couvert engine("Mini S.T.A.L.K.E.R - Map Editor",800,600);
	SDL_Surface *screen = engine.Screen();
	SDL_Event e;
	int run = 1;

	MapSystem msys;

	msys.Load("test.txt");

	int cx = 0,cy = 0;

	while(run)
	{
		clear(screen);

		while(engine.PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					run = 0;
					break;
				case SDLK_LEFT:
					--cx;
					break;
				case SDLK_RIGHT:
					++cx;
					break;
				case SDLK_UP:
					++cy;
					break;
				case SDLK_DOWN:
					--cy;
					break;
				}
				break;
			case SDL_QUIT:
				run = 0;
				break;
			}
		}

		msys.Draw(screen,cx,cy);

		SDL_Flip(screen);
	}

	return 0;
}
