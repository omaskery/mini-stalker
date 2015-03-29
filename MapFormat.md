# Map Structure Overview #

The map format has had a complete re-write. Mainly because of recent, um, breakthroughs in my coding ability relating to enterpretting information from files/networks etc. I had a week long holiday in cornwall where I had plenty of time (which most people would spend in the sun) in which to finally get some serious work done on this project.

The result of my holiday was a complete map editor, capable of loading and saving to the following map format description.

Essentially the premise of the map format is as such:

  * An ID number (four bytes, unsigned integer) to determine the type of data in this block/chunk/segment
  * A four byte unsigned number which determines the length of the block/chunk/segment in bytes.
  * The data/block/chunk/segment (whatever it is to be referred to).

Using a simple strategy-pattern-like polymorphic loader, you load a file in binary into a char array. You then essentially break the array up into "blocks" or "chunks" which can be handled seperately by polymorphic classes.

The polymorphic classes will be associated with an "ID" number, relating to the ID number that precedes a "chunk". This means that, essentially, there can be any kind of chunk added to the map load/save system at any time.

Each polymorphic class has two main methods, virtual void load(map &m, int size, char **chunk) and virtual void save(map &m, std::ofstream &f).**

There are, in the MapSystem class i've written, five basic chunks build in.

  * ID 0: width and height
  * ID 1: tile data
  * ID 2: area data
  * ID 3: positions/entities
  * ID 4: tile sprite data

These seperate chunks will now be described seperately in more detail.

> ## Width and Height ##

Width and height is stored simply as eight bytes - four bytes for each integer - not including the chunk header.

> ## Tile Data ##

Tile data is essentially just a series of numbers which will refer directly to a tile sprite/image/bitmap. There are width **height tiles, obviously, so there will be width** height values. Each value in the Tile Data section is a four byte integer.

> ## Area Data ##

An area is a simple struct consisting of an x and y co-ordinate and a width and height value. Because there are, in this game, two types of area (or uses of them) they are then grouped into "Areas" structs which encase a group of "area" struct.

Essentially:

```
struct area
{
	int x,y,w,h;
};

struct areas
{
	std::vector<area> a;
	std::string name;
};
```

Therefore an "Area" chunk in the map file in fact represents an entire group of "area" structs.

Because of the way strings and vectors obviously use internal pointers and memory allocation bla bla bla, we don't have the luxury of just writing entire "areas" structs to the file - this to many of you probably seems obvious, however to some (such as myself before recent months) this may not seem so obvious.

Therefore the area chunk consists of this:

  * An integer of four bytes representing the length of areas::name
  * The actual string to be placed inside of areas::name
  * Another integer of four bytes representing the number of "area" structs to load
  * A series of "area" structs.

The third bullet point above determines how many "area" structs (point 4) will be in this chunk. Fortunately an area struct is quite happy to be written/read directly to and from a file and therefore it is possible to simply cast some pointers to read this data.

> ## Position/Entity data ##

Again, the position struct:

```
struct position
{
	int x,y,dataA,dataB;
};
```

It is another struct which can be written/read directly to and from files - which is ever so convenient.

However, this chunk/block does not consist of one position struct, it actually represents every position struct on the map.

This chunk/block has two parts

  * Four byte integer dictating the number of positions/entities on the map
  * A series of position structs written directly to the file

This means that it's very easy to load/save as it just needs one integer (the number of positions) followed by all of the position structs written directly to the file.

Also, if your curious as to what the values in this struct are actually for then refer to the map system structs page.

> ## Tile Sprite Data ##

All of the previous chunks have been rare in a map file, most needing only one instance in a map file and two for areas (safe "areas" and bound "areas"), however the tile sprite chunk is likely to occur several times in a map file.

Each tile sprite chunk contains information regarding a tile sprite's filename (such as "grass.bmp") and it's ID number (which is used by the Tile Data chunk to create a visual map).

It is represented as such:

  * A four byte integer representing the ID number of the sprite
  * A four byte integer representing the length of the bitmap filename
  * The actual string (e.g: "grass.bmp")