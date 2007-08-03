void clear(SDL_Surface *s)
{
	int length = s->w * s->h;
	for(int i = 0; i < length; ++i)
	{
		Uint32 *bufp = (Uint32*) s->pixels + i;
		*bufp = 0;
	}
}

bool isPressed(int x, int y, int w, int h)
{
	int mx,my,button;
	button = SDL_GetMouseState(&mx,&my);
	if(!(button & SDL_BUTTON_LEFT)) return false;

	if(mx >= x && mx <= x+w && my >= y && my <= y+h) return true;
	else return false;
}

std::string toStr(int integer)
{
	std::stringstream convert;
	convert << integer;
	return convert.str();
}

int toInt(const std::string &str)
{
	std::stringstream convert(str);
	int ret;
	convert >> ret;
	return ret;
}

float toFloat(const std::string &str)
{
	std::stringstream convert(str);
	float ret;
	convert >> ret;
	return ret;
}

double toDouble(const std::string &str)
{
	std::stringstream convert(str);
	double ret;
	convert >> ret;
	return ret;
}
