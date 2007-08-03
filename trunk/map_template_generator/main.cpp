#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "MapSystem.h"

int main()
{
	map generated;

	std::cout << "Mini S.T.A.L.K.E.R map template generator v1.0\n";
	std::cout << "Program will output a blank map of the specified dimensions\n";
	std::cout << "Consisting of tiles with the id '0' entirely.\n";
	std::cout << "Four bounding boxes will be placed automatically on each edge.\n\n";
	std::cout << "To continue press any key:\n";
	char ignore_me;
	std::cin.get(ignore_me);

	std::cout << "\n\n\n";

	std::vector<std::string> filenames;
	int width, height;

	std::cout << "Enter the maps width:\n> ";
	std::cin >> width;

	std::cout << "\nEnter the maps height:\n> ";
	std::cin >> height;

	std::cout << "\nNow enter a list of bitmap filenames that will be used\n";
	std::cout << "as the tiles. When you want to end the list enter 'done' instead of a filename.\n";

	std::string input;
	while(input != "done")
	{
		std::cout << "> ";
		std::cin >> input;
		if(input != "done")
		{
			filenames.push_back(input);
		}
	}

	std::cout << "\n";

	std::cout << "A map of " << width << " by " << height << " tiles will now be generated\n";
	std::cout << "using the following bitmaps:\n";

	for(unsigned int i = 0; i < filenames.size(); ++i) std::cout << filenames[i] << "\n";

	std::cout << "Press any key to continue:\n";
	std::cin.get(ignore_me);

	std::cout << "Generating...\n";

	generated.w = width;
	generated.h = height;
	
	for(unsigned int i = 0; i < filenames.size(); ++i)
	{
		tile temp;

		temp.ID = i;
		temp.file = filenames[i];
		temp.sprite = SDL_LoadBMP(filenames[i].c_str());

		if(temp.sprite == NULL)
		{
			std::cout << "unable to load bitmap " << i << ", '" << filenames[i] << "'.\n";
			std::cout << "Aborting.\n";
			std::cin.get(ignore_me);
			return 0;
		}

		generated.sprites.push_back(temp);
	}

	std::cout << "Map data generated, please enter the name of the file to write to:\n> ";
	std::getline(std::cin,input);

	std::ofstream f(input.c_str(),std::ios::binary);
	if(!f)
	{
		std::cout << "Error, unable to access file '" << input << "'.\n";
		std::cout << "Aborting.\n";
		return 0;
	}

	write_dim(f,width,height);
	//tiles
	write_tiles(f,width,height,0);
	//areas
	write_areas_header(f,"safe",0);
	write_areas_header(f,"bound",4);
	write_area(f,0,0,10,height*32);               //across the left
	write_area(f,10,0,width*32-20,10);            //across the top
	write_area(f,10,height*32-10,width*32-20,10); //across the bottom
	write_area(f,width*32-10,0,10,height*32);     //across the right
	//positions
	write_position_header(f,0);
	//tile
	for(unsigned int i = 0; i < filenames.size(); ++i)
	{
		write_tile(f,filenames[i],i);
	}

	f.close();

	return 0;
}
