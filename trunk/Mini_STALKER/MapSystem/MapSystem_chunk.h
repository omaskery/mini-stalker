class chunk_handler
{
protected:
public:
	virtual void load(map &m, int size, char *chunk)
	{
		printf("%i: %s\n",size,chunk);
	}
	virtual void save(map &m, std::ofstream &file) = 0;
};
