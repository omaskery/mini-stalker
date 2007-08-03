void clear(SDL_Surface *s)
{
	int length = s->w * s->h;
	for(int i = 0; i < length; ++i)
	{
		Uint32 *bufp = (Uint32*) s->pixels + i;
		*bufp = 0;
	}
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

double pythagorus(double x1, double y1, double x2, double y2)
{
	double w = std::max(x1,x2) - std::min(x1,x2);
	double h = std::max(y1,y2) - std::min(y1,y2);

	return sqrt(w*w + h*h);
}