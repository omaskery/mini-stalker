struct value
{
	std::string name,val;

	value()
	{
	}
	value(const std::string &n, const std::string &v) : name(n), val(v)
	{
	}
	void get(int *i)
	{
		*i = toInt(val);
	}
	void get(float *f)
	{
		*f = toFloat(val);
	}
	void get(double *d)
	{
		*d = toDouble(val);
	}
	void get(std::string *s)
	{
		*s = val;
	}
};

struct block
{
	std::vector<block> blocks;
	std::vector<value> values;
	std::string name;

	value *get_value(const std::string &name)
	{
		for(unsigned int i = 0; i < values.size(); ++i)
		{
			if(values[i].name == name) return &values[i];
		}

		return NULL;
	}
	block *get_block(const std::string &name)
	{
		for(unsigned int i = 0; i < blocks.size(); ++i)
		{
			if(blocks[i].name == name) return &blocks[i];
		}

		return NULL;
	}
};

#include <iostream>

class config
{
	std::vector<block> blocks;
	std::string filename;
	void remove_outer_spaces(std::string &str)
	{
		unsigned int first = str.find_first_not_of(" ");
		unsigned int last;

		str.erase(0,first);

		last = str.size()-1;

		while(last >= 0 && str[last] == ' ')
		{
			--last;
		}

		str.erase(last);
	}
	int load_contents(std::string *str)
	{
		std::ifstream f(filename.c_str());
		if(!f) return -1;

		size_t size;
		f.seekg(0,std::ios::end);
		size = f.tellg();
		f.seekg(0,std::ios::beg);

		char *read = new char[size];
		f.read(read,size);

		*str = read;
		delete read;

		f.close();
		return 0;
	}
	block load_block(const std::string &name, const std::string &Block)
	{
		block temp;
		temp.name = name;

		int pos = 0;
		while(1)
		{
			int b_pos = pos;
			pos = Block.find("block",pos);
			if(pos == std::string::npos)
			{
				pos = b_pos;
				break;
			}
			pos = Block.find("\"",pos);
			int epos = Block.find("\"",pos+1);

			++pos;
			std::string Block_name = Block.substr(pos,epos-pos);

			pos = Block.find("{",epos);
			int scope = 1;

			int start = pos;
			while(pos < Block.size() && scope > 0)
			{
				if(scope > 0) ++pos;

				if(Block[pos] == '{') ++scope;
				if(Block[pos] == '}') --scope;
			}

			int end = pos;

			if(Block[start] == '{') ++start;
			if(Block[end] == '}') --end;

			std::string Block_contents = Block.substr(start,end-start);

			temp.blocks.push_back(load_block(Block_name,Block_contents));
		}
		
		while(1)
		{
			pos = Block.find(":",pos);
			if(pos == std::string::npos) break;
			int e_name = Block.find_last_not_of(" ",pos-1);
			int s_name = Block.find_last_of(" ",e_name);

			++s_name;
			++e_name;

			std::string value_name = Block.substr(s_name,e_name-s_name);

			pos = Block.find_first_not_of(" ",pos+1);
			int epos = Block.find(" ",pos);
			if(Block.substr(pos,epos-pos).find("\n") != std::string::npos) epos = Block.find("\n",pos);

			std::string val = Block.substr(pos,epos-pos);
			value t;
			t.name = value_name;
			t.val = val;

			temp.values.push_back(t);
			pos = epos;
		}

		return temp;
	}
public:
	config(const std::string &file) : filename(file)
	{
	}
	block *get_block(const std::string &name)
	{
		for(unsigned int i = 0; i < blocks.size(); ++i)
		{
			if(blocks[i].name == name) return &blocks[i];
		}

		return NULL;
	}
	int load()
	{
		blocks.clear();
		std::string contents;
		if(load_contents(&contents) == -1) return -1;

		int pos = 0;
		while(1)
		{
			pos = contents.find("block",pos);
			if(pos == std::string::npos) break;
			pos = contents.find("\"",pos);
			int epos = contents.find("\"",pos+1);

			++pos;
			std::string block_name = contents.substr(pos,epos-pos);

			pos = contents.find("{",epos);
			int scope = 1;

			int start = pos;

			while(pos < contents.size() && scope > 0)
			{
				if(scope > 0) ++pos;
				
				if(contents[pos] == '{') ++scope;
				if(contents[pos] == '}') --scope;
			}

			int end = pos;

			if(contents[start] == '{') ++start;
			if(contents[end] == '}') --end;

			std::string block_contents = contents.substr(start,end-start);

			blocks.push_back(load_block(block_name,block_contents));
		}
		
		return 0;
	}
};
