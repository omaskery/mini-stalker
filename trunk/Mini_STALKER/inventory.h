struct item
{
	std::string name, description;
	int weight, price, ID;
};

struct inv_slot
{
	item i;

	int total_weight()
	{
		return i.weight * quantity;
	}
	inv_slot get(int number = 1)    //take items from this item slot
	{
		inv_slot ret;
		if(number > quantity)
		{
			ret.quantity = -1;
			return ret;
		}

		ret.i = i;
		ret.quantity = number;
		quantity -= number;

		return ret;
	}
	void give(inv_slot &item)      //give items to this item slot
	{
		if(i.ID != item.i.ID) return;

		quantity += item.quantity;
	}
	void give(const item &it, int q)  //"
	{
		if(it.ID != i.ID) return;
		
		quantity += q;
	}
	bool empty()
	{
		return (quantity <= 0);
	}
private:
	int quantity;
};

struct inventory
{
	std::vector<inv_slot> items;
	int money;
	
	int total_weight()
	{
		int ret = 0;
		for(unsigned int i = 0; i < items.size(); ++i) ret += items[i].total_weight();
		return ret;
	}

	void give(inv_slot &it)
	{
		for(unsigned int i = 0; i < items.size(); ++i)
		{
			if(items[i].i.ID == it.i.ID)
			{
				items[i].give(it);
				return;
			}
		}

		items.push_back(it);
	}
	inv_slot take(const item &it, int number = 1)
	{
		for(unsigned int i = 0; i < items.size(); ++i)
		{
			if(items[i].i.ID == it.ID)
			{
				return items[i].get(number);
			}
		}

		inv_slot error;
		error.give(item(),-1);
		return error;
	}
};
