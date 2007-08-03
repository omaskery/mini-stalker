#include <math.h>

void render_light(SDL_Surface *s, int X, int Y, int radius, Colour &C)
{
	int r = C.r, g = C.g, b = C.b;

	double distance = 0;
	double alpha = 0;
	int x = X-radius;
	int y = Y-radius;
	int w = 0, h = 0;

	double step = 255 / radius;

	for(; y < Y + radius; ++y)
	{
		if(y < 0) continue;
		if(y >= 600) break;

		for(x = X-radius; x < X + radius; ++x)
		{
			if(x < 0) continue;
			if(x >= 800) break;

			w = X-x;
			h = Y-y;

			distance = sqrt((double)(w*w)+(h*h));

			alpha = 255-step*distance;

			if(alpha < 0) alpha = 0;
			if(alpha > 255) alpha = 255;

			if(alpha)
			{
				Pixel(s,x,y,Colour(r,g,b,(int)alpha));
			}
		}
	}
}
