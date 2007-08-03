#define TRIGGER_DISTANCE 32

class map_renderer
{
	std::vector<position> *positions;
	std::vector<position> lights;
	areas *safe, *bound;
	config map_ids;
	MapSystem msys;

	int left_id, right_id, up_id, down_id;

	bool jump_protection;

	bool test_jump(const std::string &map_name, int nx, int ny, int nw, int nh)
	{
		MapSystem test;
		if(test.load(map_name)) return true;

		printf("testing %i,%i %i,%i\n",nx,ny,nw,nh);

		return test.getArea("bound")->collision(nx,ny,nw,nh);
	}
public:
	map_renderer() : map_ids("mapids.txt"), jump_protection(0)
	{
		map_ids.load();
	}
	void load(const std::string &str)
	{
		lights.clear();

		msys.load(str);
		positions = &msys.getMap()->pos;
		safe = msys.getArea("safe");
		bound = msys.getArea("bound");

		up_id = -1;
		down_id = -1;
		left_id = -1;
		right_id = -1;

		for(unsigned int i = 0; i < positions->size(); ++i)
		{
			switch(positions->at(i).dataA)
			{
			case LIGHT :
			case CAMP_FIRE :
				{
					lights.push_back(positions->at(i));
				} break;
			case MAP_CHANGE_UP:
				{
					up_id = positions->at(i).dataB;
				} break;
			case MAP_CHANGE_DOWN:
				{
					down_id = positions->at(i).dataB;
				} break;
			case MAP_CHANGE_LEFT:
				{
					left_id = positions->at(i).dataB;
				} break;
			case MAP_CHANGE_RIGHT:
				{
					right_id = positions->at(i).dataB;
				} break;
			}
		}
	}
	const std::vector<position> *entities()
	{
		return positions;
	}
	bool hit_bound(int x, int y, int w, int h)
	{
		return bound->collision(x,y,w,h);
	}
	bool is_safe(int x, int y, int w, int h)
	{
		return safe->collision(x,y,w,h);
	}
	int w()
	{
		return msys.getMap()->w;
	}
	int h()
	{
		return msys.getMap()->h;
	}
	void render(SDL_Surface *dest)
	{
		msys.draw(dest,cam.x,cam.y);
	}
	void render_lights(SDL_Surface *dest)
	{
		for(unsigned int i = 0; i < lights.size(); ++i)
		{
			if(lights[i].x-cam.x < 0 || lights[i].x-cam.x >= dest->w || lights[i].y-cam.y < 0 || lights[i].y-cam.y >= dest->h) continue;

			if(lights[i].dataA == LIGHT)
			{
				render_light(dest,lights[i].x-cam.x,lights[i].y-cam.y,lights[i].dataB,Colour(255,255,0));
			}
			else
			{
				render_light(dest,lights[i].x-cam.x,lights[i].y-cam.y,lights[i].dataB,Colour(255,200,0));
			}
		}
	}
	void check_triggers(player &p)
	{
		bool near = false;

		if(p.x <= 16 && left_id != -1)
		{
			value *v = map_ids.get_block("ids")->get_value(toStr(left_id));
			
			std::string map_name;
			if(v) v->get(&map_name);
			else
			{
				printf("invalid map trigger %i, ignoring.\n", left_id);
				return;
			}

			if(!jump_protection)
			{
				if(!test_jump(map_name,((msys.getMap()->w * 32) - p.w) - 32,(int)p.y,p.w,p.h))
				{
					load(map_name);
					p.x = (float) ((msys.getMap()->w * 32) - p.w) - 32;

					cam.x = (int)p.x-400;
					cam.y = (int)p.y-300;

					if(cam.x < 0) cam.x = 0;
					if(cam.y < 0) cam.y = 0;
					if(cam.x >= w()*32-800) cam.x = w()*32-800;
					if(cam.y >= h()*32-600) cam.y = h()*32-600;

					p.rect.x = (Sint16)p.x-cam.x;
					p.rect.y = (Sint16)p.y-cam.y;
					
					jump_protection = true;
				}
			}
			
			near = true;
		}
		else if(p.y <= 16 && up_id != -1)
		{
			value *v = map_ids.get_block("ids")->get_value(toStr(up_id));
			
			std::string map_name;
			if(v) v->get(&map_name);
			else
			{
				printf("invalid map trigger %i, ignoring.\n", up_id);
				return;
			}

			if(!jump_protection)
			{
				if(!test_jump(map_name,(int)p.x,msys.getMap()->h*32-p.h-32,p.w,p.h))
				{
					load(map_name);
					p.y = (float) ((msys.getMap()->w * 32) - p.w) - 32;

					cam.x = (int)p.x-400;
					cam.y = (int)p.y-300;

					if(cam.x < 0) cam.x = 0;
					if(cam.y < 0) cam.y = 0;
					if(cam.x >= w()*32-800) cam.x = w()*32-800;
					if(cam.y >= h()*32-600) cam.y = h()*32-600;

					p.rect.x = (Sint16)p.x-cam.x;
					p.rect.y = (Sint16)p.y-cam.y;
					
					jump_protection = true;
				}
			}
			
			near = true;
		}
		else if(p.x >= ((msys.getMap()->w * 32) - p.w) - 16 && right_id != -1)
		{
			value *v = map_ids.get_block("ids")->get_value(toStr(right_id));
			
			std::string map_name;
			if(v) v->get(&map_name);
			else
			{
				printf("invalid map trigger %i, ignoring.\n", right_id);
				return;
			}

			if(!jump_protection)
			{
				if(!test_jump(map_name,32,(int)p.y,p.w,p.h))
				{
					load(map_name);
					p.x = 32;

					cam.x = (int)p.x-400;
					cam.y = (int)p.y-300;

					if(cam.x < 0) cam.x = 0;
					if(cam.y < 0) cam.y = 0;
					if(cam.x >= w()*32-800) cam.x = w()*32-800;
					if(cam.y >= h()*32-600) cam.y = h()*32-600;

					p.rect.x = (Sint16)p.x-cam.x;
					p.rect.y = (Sint16)p.y-cam.y;
					
					jump_protection = true;
				}
			}
			
			near = true;
		}
		else if(p.y >= ((msys.getMap()->h * 32) - p.h) - 16 && down_id != -1)
		{
			value *v = map_ids.get_block("ids")->get_value(toStr(down_id));
			
			std::string map_name;
			if(v) v->get(&map_name);
			else
			{
				printf("invalid map trigger %i, ignoring.\n", down_id);
				return;
			}

			if(!jump_protection)
			{
				if(!test_jump(map_name,(int)p.x,32,p.w,p.h))
				{
					load(map_name);
					p.y = 32;

					cam.x = (int)p.x-400;
					cam.y = (int)p.y-300;

					if(cam.x < 0) cam.x = 0;
					if(cam.y < 0) cam.y = 0;
					if(cam.x >= w()*32-800) cam.x = w()*32-800;
					if(cam.y >= h()*32-600) cam.y = h()*32-600;

					p.rect.x = (Sint16)p.x-cam.x;
					p.rect.y = (Sint16)p.y-cam.y;
					
					jump_protection = true;
				}
			}

			near = true;
		}

		if(!near) jump_protection = false;
	}
};
