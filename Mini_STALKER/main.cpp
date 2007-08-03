#include "includes.h"

int main()
{
	Couvert engine("Mini S.T.A.L.K.E.R",800,600);
	SDL_Surface *screen = engine.Screen();
	Text text("my_font.bmp");
	map_renderer map_render;
	
	SDL_Event e;
	int run = 1;

	unsigned int timer = clock();
	int fps = 0, frames = 0;
	
	map_render.load("map1.txt");

	player plr("player.bmp");
	plr.x = 32;
	plr.y = 128;
	plr.speed = 1.0;

	bool fullscreen = false;

	while(run)
	{
		while(engine.PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_KEYDOWN:
				{
					switch(e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						run = 0;
						break;
					case SDLK_F11:
						{
							fullscreen = !fullscreen;
							if(fullscreen) screen = SDL_SetVideoMode(800,600,32,SDL_DOUBLEBUF|SDL_FULLSCREEN);
							else screen = SDL_SetVideoMode(800,600,32,SDL_DOUBLEBUF);
						} break;
					}

					plr.handle(e);
				} break;
			case SDL_KEYUP :
				{
					/*switch(e.key.keysym.sym)
					{
					}*/

					plr.handle(e);
				} break;
			case SDL_QUIT:
				run = 0;
				break;
			}
		}

		int test_x,test_y;
		plr.next(&test_x,&test_y);

		if(map_render.hit_bound((int)plr.x,(int)plr.y,plr.w,plr.h)) printf("collision in normal position, wtf?\n");

		if(!map_render.hit_bound(test_x,test_y,plr.w,plr.h))
		{
			plr.move();

			cam.x = test_x-400;
			cam.y = test_y-300;

			if(cam.x < 0) cam.x = 0;
			if(cam.y < 0) cam.y = 0;
			if(cam.x >= map_render.w()*32-800) cam.x = map_render.w()*32-800;
			if(cam.y >= map_render.h()*32-600) cam.y = map_render.h()*32-600;
		}

		map_render.check_triggers(plr);

		map_render.render(screen);

		SDL_BlitSurface(plr.sprite,NULL,screen,&plr.rect);

		map_render.render_lights(screen);

		text.draw(screen,5,5,"FPS: " + toStr(fps));
		SDL_Flip(screen);

		++frames;

		if(clock()-timer >= 1000)
		{
			fps = frames;
			frames = 0;
			timer = clock();
		}
	}

	return 0;
}
