#include <string>

class Text
{
	SDL_Surface *font;
public:
	Text(const std::string &font_file)
	{
		font = SDL_LoadBMP(font_file.c_str());
		if(font) SDL_SetColorKey(font,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(font->format,0,0,0));
	}
	void draw(SDL_Surface *s, int x, int y, const std::string &text, int alpha = 255)
	{
		if(alpha > 255) alpha = 255;
		else if(alpha < 0) alpha = 0;
		if(!alpha) return;

		SDL_SetAlpha(font,SDL_SRCALPHA|SDL_RLEACCEL,alpha);

		int src_x = 0;
		int x_offset = 0;
		for(unsigned int i = 0; i < text.size(); ++i)
		{
			char c = text[i];
			SDL_Rect src, dest;
			
			src.x = (int)c * 9;
			src.y = 0;
			src.w = 9;
			src.h = 13;

			dest.x = x+x_offset;
			dest.y = y;
			dest.w = 9;
			dest.h = 13;

			if(src_x != -1) SDL_BlitSurface(font,&src,s,&dest);

			x_offset += 9;
		}

		SDL_SetAlpha(font,0,alpha);
	}
};
