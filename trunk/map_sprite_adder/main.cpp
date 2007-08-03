#include "MapSystem.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	MapSystem msys;

	std::vector<std::string> bitmaps;
	std::string mapfile;

	if(argc > 1)
	{
		mapfile = argv[1];
		std::cout << "input: " << mapfile << "\n";
	}

	for(int i = 2; i < argc; ++i)
	{
		bitmaps.push_back(argv[i]);
		std::cout << "bitmap: " << bitmaps.back() << "\n";
	}

	if(mapfile == "")
	{
		std::cout << "Enter the filename of the map you wish to add sprites to:\n> ";
		std::getline(std::cin,mapfile);
	}

	if(!bitmaps.size())
	{
		std::cout << "Enter a list of bitmap filenames to add to the map.\n";
		std::cout << "To stop entering filenames, enter 'done' instead of a filename and press enter.\n";

		std::string input;
		while(input != "done")
		{
			getline(cin,input);
			if(input != "done") bitmaps.push_back(input);
		}
	}

	std::cout << "Loading map file...\n";

	if(msys.load(mapfile))
	{
		cout << "Unable to load map file. Aborting\n";
		cin.get();
		return 0;
	}

	std::cout << "Adding sprites...\n";

	for(unsigned int i = 0; i < bitmaps.size(); ++i)
	{
		msys.addSprite(bitmaps[i]);
	}

	std::cout << "Saving...\n";

	msys.save(mapfile);

	std::cout << "Success.\n";
	
	return 0;
}
