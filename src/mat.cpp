#include "mat.h"

/*
bool *elem {};
size_t n = 0;

//default constructor
mat()
{
	elem = new bitset<5>{};
	n=1;
}

//destructor
~mat()
{
	delete[] elem;
}

//copy constructor
mat(const mat& m)
{
	n = m.n;
	elem = new bitset<5> [n]{};
	for (size_t y =0; y < n; y++)
	{
		elem[x] = m.get(y,x);
	}
}

//phrase constructor
mat(const phrase& p)
{
	n = p.n;
	elem = new bitset [n]{};
	size_t c = 0;
	for (size_t y = 0; y < n; y++)
	{
		word this_word = p.get(y);
		for (size_t z = 0; z < this_word.n; z++)
		{
				elem[c] = (this_word.get(z)).get(x);
				c++;
			}
		}
	}
}

//copy assignment
mat& operator=(const mat& m)
{
	delete[] elem;
	n = m.n;
	elem = new bool [5*n]{};
	for (size_t y =0; y < n; y++)
	{
		for (size_t x =0; x<5; x++){
			elem[5*y + x] = m.get(y,x);
		}
	}
	return *this;
}

//getters
bool get(size_t y, size_t x) const
{
	return elem[y*5 + x];
}

bool operator()(size_t y, size_t x) const
{
	return elem[y*5 + x];
}
		*/