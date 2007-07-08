#include "CouvertEngine.h"

int main()
{
	Couvert engine("Mini S.T.A.L.K.E.R - Map Editor",800,600);
	SDL_Surface *screen = engine.Screen();
	SDL_Event e;
	int run = 1;

	while(run)
	{
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
				}
				break;
			case SDL_QUIT:
				run = 0;
				break;
			}
		}
	}

	return 0;
}
