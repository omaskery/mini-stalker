#include <cstdlib>
#include <SDL.h>
#include <string>

#undef main

struct Colour
{
	Uint8 r,g,b,a;

	Colour(Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255) : r(R), g(G), b(B), a(A)
	{
	}
};

class Bitmap
{
	SDL_Surface *bmp;
	bool lock;
public:
	Bitmap() : lock(0)
	{
	}
	Bitmap(const std::string &filename) : lock(0)
	{
		Load(filename);
	}
	~Bitmap()
	{
		if(lock) SDL_FreeSurface(bmp);
	}
	void SetMask(const Colour &C)
	{
		SDL_SetColorKey(bmp,SDL_SRCCOLORKEY | SDL_RLEACCEL,SDL_MapRGB(bmp->format,C.r,C.g,C.b));
	}
	void Draw(SDL_Surface *screen, int x, int y)
	{
		SDL_Rect src, dest;

		src.x = 0;
		src.y = 0;
		src.w = bmp->w;
		src.h = bmp->h;

		dest.x = x;
		dest.y = y;
		dest.w = bmp->w;
		dest.h = bmp->h;

		SDL_BlitSurface(bmp,&src,screen,&dest);
	}
	void Load(const std::string &filename)
	{
		if(lock) return;

		SDL_Surface *temp = SDL_LoadBMP(filename.c_str());

		if(!bmp)
		{
			fprintf(stderr,"Unable to load bitmap: %s\n",SDL_GetError());
			return;
		}

		bmp = SDL_DisplayFormat(temp);

		printf("loaded %s\n",filename.c_str());

		lock = true;
	}
};

class Couvert
{
	SDL_Surface *screen;
	Uint8 *keys;
	bool quit;
public:
	Couvert(const std::string &window_title, int width, int height, int flags = SDL_DOUBLEBUF) : quit(false)
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			fprintf(stderr,"Unable to initialise SDL: %s\n",SDL_GetError());
		}

		screen = SDL_SetVideoMode(width,height,32,flags);

		if(screen == NULL)
		{
			fprintf(stderr,"Unable to set video mode: %s\n",SDL_GetError());
		}

		keys = SDL_GetKeyState(NULL);
		SDL_WM_SetCaption(window_title.c_str(),window_title.c_str());
	}
	int PollEvent(SDL_Event *e)
	{
		return SDL_PollEvent(e);
	}
	int KeyState(int key)
	{
		return keys[key];
	}
	bool LeftMouse()
	{
		return (SDL_GetMouseState(NULL,NULL) == SDL_BUTTON_LEFT);
	}
	bool RightMouse()
	{
		return (SDL_GetMouseState(NULL,NULL) == SDL_BUTTON_RIGHT);
	}
	void MousePos(int *x, int *y)
	{
		SDL_GetMouseState(x,y);
	}
	int MouseState(int *x, int *y)
	{
		return SDL_GetMouseState(x,y);
	}
	SDL_Surface *Screen()
	{
		return screen;
	}
	~Couvert()
	{
		SDL_Quit();
	}
};

Uint32 PixelAt(SDL_Surface *surface, int x, int y)
{
	Uint32 *bufp = (Uint32*)surface->pixels + y*surface->w+x;

	return *bufp;
}

void Pixel(SDL_Surface *surface, int x, int y, const Colour &C)
{
	if(x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;

	Uint32 source = SDL_MapRGB(surface->format,C.r,C.g,C.b);

	Uint32 *dest = (Uint32*) surface->pixels + y*surface->w + x;

	if(C.a == 255)
	{
		*dest = source;
		return;
	}

	Uint32 RMask = 0xFF000000, GMask = 0x00FF0000, BMask = 0x0000FF00;

	int alpha = C.a;
	int ialpha = 256 - alpha;

	*dest = (RMask & (((source & RMask) * alpha +
		(*dest & RMask) * ialpha) >>8)) |
		 (GMask & (((source & GMask) * alpha +
		(*dest & GMask) * ialpha) >>8)) |
		 (BMask & (((source & BMask) * alpha +
		(*dest & BMask) * ialpha) >>8));
}
