#include "includes.h"

int main()
{
	Couvert engine("Mini S.T.A.L.K.E.R - Map Editor",800,600);
	SDL_Surface *screen = engine.Screen();
	SDL_Surface *tools = SDL_LoadBMP("tools.bmp");
	bool space = false;
	SDL_Event e;
	int run = 1;

	SDL_EnableUNICODE(1);

	Text text("my_font.bmp");

	unsigned int timer = clock();
	int fps = 0,frames = 0;

	MapEditor editor(screen,2.0);
	SDL_Rect tool_rect;
	tool_rect.x = 750;
	tool_rect.y = 45;
	tool_rect.w = 50;
	tool_rect.h = 300;

	//make_test_map("save.txt",101,50);

	std::string filename;
	editor.getConfig()->get_block("config")->get_block("defaults")->get_value("last")->get(&filename);
	if(filename == "") filename = "save.txt";

	if(editor.load(filename))
	{
		printf("unable to load file\n");
	}

	while(run)
	{
		while(engine.PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				run = 0;
				break;
			case SDL_KEYDOWN :
				{
					if(e.key.keysym.sym == SDLK_ESCAPE) run = 0;
					else if(e.key.keysym.sym == SDLK_SPACE) space = true;
				} break;
			case SDL_KEYUP :
				{
					if(e.key.keysym.sym == SDLK_SPACE) space = false;
				} break;
			}

			editor.handle_event(e);
		}

		if(space)
		{
			if(isPressed(tool_rect.x+7,tool_rect.y+7  ,36,36)) editor.set_tool(SAFE_ZONE);
			if(isPressed(tool_rect.x+7,tool_rect.y+46 ,36,36)) editor.set_tool(BOUND_ZONE);
			if(isPressed(tool_rect.x+7,tool_rect.y+84 ,36,36)) editor.set_tool(TILE_EDIT);
			if(isPressed(tool_rect.x+7,tool_rect.y+123,36,36)) editor.set_tool(PLACE_ENT);
		}

		editor.render();
		editor.update();
		if(space)
		{
			SDL_BlitSurface(tools,NULL,screen,&tool_rect);

			text.draw(screen,723,5,"FPS: " + toStr(fps));
		}

		SDL_Flip(screen);

		++frames;

		if(clock()-timer >= 1000)
		{
			fps = frames;
			frames = 0;
			timer = clock();
		}
	}

	editor.save(filename);

	SDL_FreeSurface(tools);

	return 0;
}
