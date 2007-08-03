#include "MapSystem.h"

enum tool_mode_enum
{
	SAFE_ZONE = 1,
	BOUND_ZONE,
	TILE_EDIT,
	PLACE_ENT,
};

struct textbox
{
	std::string text;
	bool active;
	
	textbox(const std::string &t = "") : text(t), active(0)
	{
	}
};

struct camera
{
	float x,y,mx,my;

	camera() : x(0),y(0),mx(0),my(0)
	{
	}
	camera(float X, float Y) : x(X), y(Y), mx(0), my(0)
	{
	}
};

class MapEditor
{
	SDL_Surface *safe_hud, *bound_hud, *tile_hud, *ent_hud;
	areas *safe, *bound;
	MapSystem map_system;
	SDL_Surface *dest;
	float speed;
	config conf;
	camera cam;
	Text text;

	int w,h, mouse_x,mouse_y,mouse_p_button,mouse_button, tool_mode;

	int x1,y1,x2,y2;
	int tile_kind;
	SDL_Surface *current_tile;
	SDL_Rect c_tile_rect;
	textbox dataA,dataB;
	
	bool active, shift, space;

	void safe_zone()
	{
		if(space)
		{
		}
		else if(mouse_button == SDL_BUTTON_RIGHT)
		{
			std::vector<area>::iterator iter = safe->a.begin();
			while(iter != safe->a.end())
			{
				if(mouse_x >= iter->x-cam.x && mouse_x <= iter->x+iter->w-cam.x && mouse_y >= iter->y-cam.y && mouse_y <= iter->y+iter->h-cam.y)
				{
					safe->a.erase(iter);
					break;
				}

				if(iter != safe->a.end()) ++iter;
			}
		}
		else if(mouse_button == SDL_BUTTON_LEFT)
		{
			if(x1 == -1)
			{
				x1 = mouse_x;
				y1 = mouse_y;
			}
			else
			{
				SDL_Rect toMouse;
				toMouse.x = std::min(mouse_x,x1);
				toMouse.y = std::min(mouse_y,y1);
				toMouse.w = std::max(mouse_x,x1) - std::min(mouse_x,x1);
				toMouse.h = std::max(mouse_y,y1) - std::min(mouse_y,y1);

				SDL_FillRect(dest,&toMouse,SDL_MapRGB(dest->format,0,255,0));
			}
		}
		else
		{
			if(x1 != -1)
			{
				x2 = mouse_x;
				y2 = mouse_y;

				area new_safe;
				new_safe.x = std::min(x1,x2) + cam.x;
				new_safe.y = std::min(y1,y2) + cam.y;
				new_safe.w = std::max(x1,x2) - std::min(x1,x2);
				new_safe.h = std::max(y1,y2) - std::min(y1,y2);

				safe->a.push_back(new_safe);

				x1 = -1;
			}
		}

		for(unsigned int i = 0; i < safe->a.size(); ++i)
		{
			area &a = safe->a[i];

			if(a.x+a.w-cam.x < 0 || a.y+a.h-cam.y < 0 || a.x-cam.x >= dest->w || a.y-cam.y >= dest->h) continue;

			for(int y = a.y-(int)cam.y; y < a.y + a.h - (int)cam.y; ++y)
			{
				if(y > dest->h) break;
				if(y < 0) continue;

				for(int x = a.x - (int)cam.x; x < a.x + a.w - (int)cam.x; ++x)
				{
					if(x > dest->w) break;
					if(x < 0) continue;

					Pixel(dest,x,y,Colour(0,255,0,128));
				}
			}
		}

		SDL_BlitSurface(safe_hud,NULL,dest,NULL);
	}
	void bound_zone()
	{
		if(space)
		{
		}
		else if(mouse_button == SDL_BUTTON_RIGHT)
		{
			std::vector<area>::iterator iter = bound->a.begin();
			while(iter != bound->a.end())
			{
				if(mouse_x >= iter->x-cam.x && mouse_x <= iter->x+iter->w-cam.x && mouse_y >= iter->y-cam.y && mouse_y <= iter->y+iter->h-cam.y)
				{
					bound->a.erase(iter);
					break;
				}

				if(iter != bound->a.end()) ++iter;
			}
		}
		else if(mouse_button == SDL_BUTTON_LEFT)
		{
			if(x1 == -1)
			{
				x1 = mouse_x;
				y1 = mouse_y;
			}
			else
			{
				SDL_Rect toMouse;
				toMouse.x = std::min(mouse_x,x1);
				toMouse.y = std::min(mouse_y,y1);
				toMouse.w = std::max(mouse_x,x1) - std::min(mouse_x,x1);
				toMouse.h = std::max(mouse_y,y1) - std::min(mouse_y,y1);

				SDL_FillRect(dest,&toMouse,SDL_MapRGB(dest->format,255,0,0));
			}
		}
		else
		{
			if(x1 != -1)
			{
				x2 = mouse_x;
				y2 = mouse_y;

				area new_bound;
				new_bound.x = std::min(x1,x2) + cam.x;
				new_bound.y = std::min(y1,y2) + cam.y;
				new_bound.w = std::max(x1,x2) - std::min(x1,x2);
				new_bound.h = std::max(y1,y2) - std::min(y1,y2);

				bound->a.push_back(new_bound);

				x1 = -1;
			}
		}

		for(unsigned int i = 0; i < bound->a.size(); ++i)
		{
			area &a = bound->a[i];

			if(a.x+a.w-(int)cam.x < 0 || a.y+a.h-(int)cam.y < 0 || a.x-(int)cam.x >= dest->w || a.y-(int)cam.y >= dest->h) continue;

			for(int y = a.y-(int)cam.y; y < a.y + a.h - (int)cam.y; ++y)
			{
				if(y > dest->h) break;
				if(y < 0) continue;

				for(int x = a.x - (int)cam.x; x < a.x + a.w - (int)cam.x; ++x)
				{
					if(x > dest->w) break;
					if(x < 0) continue;
					Pixel(dest,x,y,Colour(255,0,0,128));
				}
			}
		}

		SDL_BlitSurface(bound_hud,NULL,dest,NULL);
	}
	void tile_edit()
	{
		int i_x = 0, i_y = 0;
		int tx = mouse_x+cam.x, ty = mouse_y+cam.y;

		if(space)
		{
			if(isPressed(183,5,34,34) && !mouse_p_button)
			{
				tile_kind = (tile_kind - 1) % (map_system.getMap()->sprites.size());
				current_tile = NULL;
			}
			else if(isPressed(257,5,34,34) && !mouse_p_button)
			{
				tile_kind = (tile_kind + 1) % (map_system.getMap()->sprites.size());
				current_tile = NULL;
			}
		}

		while(tx - 32 > 0)
		{
			tx -= 32;
			++i_x;
		}
		while(ty - 32 > 0)
		{
			ty -= 32;
			++i_y;
		}

		if(!space && mouse_button == SDL_BUTTON_LEFT && (mouse_p_button == 0 || shift))
		{
			map_system.getMap()->tiles[i_y*w+i_x] = tile_kind;
		}

		i_x = i_x * 32 - cam.x;
		i_y = i_y * 32 - cam.y;

		for(int y = i_y; y < i_y+32; ++y)
		{
			for(int x = i_x; x < i_x+32; ++x)
			{
				Pixel(dest,x,y,Colour(255,0,0,128));
			}
		}

		SDL_BlitSurface(tile_hud,NULL,dest,NULL);
		//221,6

		if(!current_tile)
		{
			for(unsigned int i = 0; i < map_system.getMap()->sprites.size(); ++i)
			{
				if(map_system.getMap()->sprites[i].ID == tile_kind)
				{
					current_tile = map_system.getMap()->sprites[i].sprite;
					break;
				}
			}
		}

		SDL_BlitSurface(current_tile,NULL,dest,&c_tile_rect);
	}
	void place_ent()
	{
		std::vector<position> &pos = map_system.getMap()->pos;

		if(space)
		{
		}
		else if(mouse_button == SDL_BUTTON_RIGHT && !mouse_p_button)
		{
			std::vector<position>::iterator iter = map_system.getMap()->pos.begin();
			while(iter != map_system.getMap()->pos.end())
			{
				if(mouse_x+cam.x >= iter->x-10 && mouse_x+cam.x <= iter->x+10 && mouse_y+cam.y >= iter->y-10 && mouse_y+cam.y <= iter->y+10)
				{
					map_system.getMap()->pos.erase(iter);
					break;
				}

				if(iter != map_system.getMap()->pos.end()) ++iter;
			}
		}
		else if(mouse_button == SDL_BUTTON_LEFT && !mouse_p_button)
		{
			position temp;

			temp.x = mouse_x + cam.x;
			temp.y = mouse_y + cam.y;
			temp.dataA = toInt(dataA.text);
			temp.dataB = toInt(dataB.text);

			pos.push_back(temp);
		}

		for(unsigned int i = 0; i < pos.size(); ++i)
		{
			if(pos[i].x-cam.x < 0 || pos[i].x-cam.x >= dest->w || pos[i].y-cam.y < 0 || pos[i].y-cam.y >= dest->h) continue;

			SDL_Rect ent;
			ent.x = pos[i].x-cam.x-10;
			ent.y = pos[i].y-cam.y-10;
			ent.w = 20;
			ent.h = 20;

			SDL_FillRect(dest,&ent,SDL_MapRGB(dest->format,0,0,255));

			text.draw(dest,ent.x-5,ent.y-10,toStr(pos[i].dataA));
			text.draw(dest,ent.x-5,ent.y+5 ,toStr(pos[i].dataB));
		}

		SDL_BlitSurface(ent_hud,NULL,dest,NULL);
		text.draw(dest,201,14,dataA.text);
		text.draw(dest,339,14,dataB.text);
	}
public:
	MapEditor(SDL_Surface *d, float speed) : conf("config.txt"),text("my_font.bmp"),dataA("0"),dataB("0"),dest(d),w(0),h(0),space(0),active(0),current_tile(0),tile_kind(0),speed(speed),mouse_button(0),tool_mode(SAFE_ZONE), shift(0)
	{
		x1 = -1;
		x2 = -1;
		y1 = -1;
		y2 = -1;

		safe_hud = SDL_LoadBMP("safe_hud.bmp");
		bound_hud = SDL_LoadBMP("bound_hud.bmp");
		tile_hud = SDL_LoadBMP("tile_hud.bmp");
		ent_hud = SDL_LoadBMP("ent_hud.bmp");

		SDL_SetColorKey(safe_hud,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(safe_hud->format,0,0,0));
		SDL_SetColorKey(bound_hud,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(bound_hud->format,0,0,0));
		SDL_SetColorKey(tile_hud,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(tile_hud->format,0,0,0));
		SDL_SetColorKey(ent_hud,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(ent_hud->format,0,0,0));

		int alpha = 230;
		SDL_SetAlpha(safe_hud,SDL_SRCALPHA|SDL_RLEACCEL,alpha);
		SDL_SetAlpha(bound_hud,SDL_SRCALPHA|SDL_RLEACCEL,alpha);
		SDL_SetAlpha(tile_hud,SDL_SRCALPHA|SDL_RLEACCEL,alpha);
		SDL_SetAlpha(ent_hud,SDL_SRCALPHA|SDL_RLEACCEL,alpha);

		c_tile_rect.x = 221;
		c_tile_rect.y = 6;
		c_tile_rect.w = 32;
		c_tile_rect.h = 32;

		conf.load();

		conf.get_block("config")->get_block("defaults")->get_value("ent_dataA")->get(&dataA.text);
		conf.get_block("config")->get_block("defaults")->get_value("ent_dataB")->get(&dataB.text);
		conf.get_block("config")->get_block("settings")->get_value("scroll_speed")->get(&speed);

		std::string conf_tool;
		conf.get_block("config")->get_block("defaults")->get_value("tool")->get(&conf_tool);
		if(conf_tool == "safe_zone") tool_mode = SAFE_ZONE;
		else if(conf_tool == "bound_zone") tool_mode = BOUND_ZONE;
		else if(conf_tool == "tile_edit") tool_mode = TILE_EDIT;
		else if(conf_tool == "place_ent") tool_mode = PLACE_ENT;
	}
	~MapEditor()
	{
		SDL_FreeSurface(safe_hud);
		SDL_FreeSurface(bound_hud);
		SDL_FreeSurface(tile_hud);
		SDL_FreeSurface(ent_hud);
	}
	config *getConfig()
	{
		return &conf;
	}
	int load(const std::string &filename)
	{
		if(map_system.load(filename)) return -1;

		w = map_system.getMap()->w;
		h = map_system.getMap()->h;
		safe = map_system.getArea("safe");
		bound = map_system.getArea("bound");
		active = true;

		return 0;
	}
	bool isActive()
	{
		return active;
	}
	void set_tool(int mode)
	{
		tool_mode = mode;
	}
	void save(const std::string &filename)
	{
		if(active) map_system.save(filename);
	}
	void handle_event(SDL_Event &e)
	{
		switch(e.type)
		{
		case SDL_MOUSEBUTTONDOWN :
			{
				mouse_x = e.button.x;
				mouse_y = e.button.y;
				mouse_button = e.button.button;

				if(space && tool_mode == PLACE_ENT)
				{
					if(isPressed(201,13,75,15)) dataA.active = true;
					else dataA.active = false;
					if(isPressed(339,13,75,15)) dataB.active = true;
					else dataB.active = false;
				}
			} break;
		case SDL_MOUSEBUTTONUP :
			{
				mouse_x = e.button.x;
				mouse_y = e.button.y;
				mouse_button = 0;
			} break;
		case SDL_KEYDOWN :
			{
				switch(e.key.keysym.sym)
				{
				case SDLK_UP :
					{
						cam.my = -speed;
					} break;
				case SDLK_DOWN :
					{
						cam.my = speed;
					} break;
				case SDLK_LEFT :
					{
						cam.mx = -speed;
					} break;
				case SDLK_RIGHT :
					{
						cam.mx = speed;
					} break;
				case SDLK_LSHIFT :
				case SDLK_RSHIFT :
					{
						shift = true;
					} break;
				case SDLK_SPACE :
					{
						space = true;
					} break;
				case SDLK_BACKSPACE :
					{
						if(dataA.active && dataA.text.size()) dataA.text.erase(dataA.text.size()-1);
						if(dataB.active && dataB.text.size()) dataB.text.erase(dataB.text.size()-1);
					} break;
				default:
					{
						if(tool_mode == PLACE_ENT)
						{
							if(e.key.keysym.unicode >= '0' && e.key.keysym.unicode <= '9' || e.key.keysym.unicode == '-')
							{
								if(dataA.active && dataA.text.size() < 8) dataA.text += e.key.keysym.unicode;
								if(dataB.active && dataB.text.size() < 8) dataB.text += e.key.keysym.unicode;
							}
						}
					} break;
				}
			} break;
		case SDL_KEYUP :
			{
				switch(e.key.keysym.sym)
				{
				case SDLK_UP :
				case SDLK_DOWN :
					{
						cam.my = 0;
					} break;
				case SDLK_LEFT :
				case SDLK_RIGHT :
					{
						cam.mx = 0;
					} break;
				case SDLK_LSHIFT :
				case SDLK_RSHIFT :
					{
						shift = false;
					} break;
				case SDLK_SPACE :
					{
						space = false;
					} break;
				}
			} break;
		}
	}
	void update()
	{
		cam.x += cam.mx;
		cam.y += cam.my;

		if(cam.x < 0) cam.x = 0;
		if(cam.y < 0) cam.y = 0;
		if(cam.x >= w*32-800) cam.x = (float)w*32-800;
		if(cam.y >= h*32-600) cam.y = (float)h*32-600;

		SDL_GetMouseState(&mouse_x,&mouse_y);
		
		switch(tool_mode)
		{
		case SAFE_ZONE : safe_zone(); break;
		case BOUND_ZONE : bound_zone(); break;
		case TILE_EDIT : tile_edit(); break;
		case PLACE_ENT : place_ent(); break;
		default:
			printf("invalid tool.\n");
		}
		
		mouse_p_button = mouse_button;
	}
	void render()
	{
		if(!active)
		{
			printf("cannot render, inactive.\n");
			return;
		}
		
		map_system.draw(dest,(int)cam.x,(int)cam.y);

		text.draw(dest,723,582,toStr(mouse_x+cam.x)+","+toStr(mouse_y+cam.y));
	}
};
