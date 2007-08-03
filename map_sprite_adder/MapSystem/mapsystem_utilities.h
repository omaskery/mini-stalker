void write_chunk(std::ofstream &f, int ID, int size, char *data)
{
	f.write((char*)&ID,4);
	f.write((char*)&size,4);
	f.write(data,size);
}

void write_int(std::ofstream &f, int integer)
{
	f.write((char*)&integer,4);
}

void write_dim(std::ofstream &f, int w, int h)
{
	write_int(f,0);
	write_int(f,8);
	write_int(f,w);
	write_int(f,h);
}

void write_tiles(std::ofstream &f, int w, int h, int v)
{
	write_int(f,1);
	write_int(f,w*h*4);
	for(int i = 0; i < w*h; ++i) write_int(f,v);
}

void write_tiles(std::ofstream &f, int w, int h, int v1, int v2)
{
	write_int(f,1);
	write_int(f,w*h*4);
	for(int i = 0; i < w*h; ++i)
	{
		if(i % 2) write_int(f,v1);
		else write_int(f,v2);
	}
}

void write_areas_header(std::ofstream &f,const std::string &name,int count)
{
	write_int(f,2);
	write_int(f,(int)name.size()+8+count*(int)sizeof(area));
	write_int(f,(int)name.size());
	f.write(name.c_str(),(int)name.size());
	write_int(f,count);
}

void write_area(std::ofstream &f, int x,int y, int w, int h)
{
	area a;
	a.x = x;
	a.y = y;
	a.w = w;
	a.h = h;
	f.write((char*)&a,sizeof(a));
}

void write_position_header(std::ofstream &f,int count)
{
	write_int(f,3);
	write_int(f,4+count*sizeof(position));
	write_int(f,count);
}

void write_position(std::ofstream &f,int x, int y, int dA, int dB)
{
	write_int(f,x);
	write_int(f,y);
	write_int(f,dA);
	write_int(f,dB);
}

void write_tile(std::ofstream &f, const std::string &name, int ID)
{
	write_int(f,4);
	write_int(f,8+(int)name.size());
	write_int(f,ID);
	write_int(f,(int)name.size());
	f.write(name.c_str(),(int)name.size());
}

void make_test_map(const std::string &filename, int width,int height)
{
	std::ofstream f(filename.c_str(),std::ios::binary);
	
	write_dim(f,width,height);
	//tiles
	write_tiles(f,width,height,0,1);
	//areas
	write_areas_header(f,"safe",2);
	write_area(f,10,10,10,10);
	write_area(f,20,20,20,20);

	write_areas_header(f,"bound",4);
	write_area(f,0,0,10,height*32);               //across the left
	write_area(f,10,0,width*32-20,10);            //across the top
	write_area(f,10,height*32-10,width*32-20,10); //across the bottom
	write_area(f,width*32-10,0,10,height*32);     //across the right
	//position
	write_position_header(f,2);
	write_position(f,400,300,0,0xFFFFFFFF);
	write_position(f,800,600,0,0xFFFFFFFF);
	//tile
	write_tile(f,"tile1.bmp",0);
	write_tile(f,"tile2.bmp",1);

	f.close();
}
