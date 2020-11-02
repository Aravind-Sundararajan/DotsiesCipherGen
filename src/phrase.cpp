#include "phrase.h"
using namespace std;
phrase::phrase()
{
	words = new word[1]{};
	n = 1;
};

phrase::phrase(const word w)
{
	words = new word[1]{};
	n = 1;
	words[0] = w;
};

//destructor
phrase::~phrase()
{
	delete[] words;
};

//copy constructor
phrase::phrase(const phrase& p)
{
	n = p.n;
	words = new word [n]{};
	for (size_t x =0; x < n; x++)
	{
		words[x] = p.get(x);
	}
};

//assignment copy
phrase& phrase::operator=(const phrase& p)
{
	delete[] words;
	n = p.n;
	words = new word[n]{};
	for (size_t x = 0; x < n; x++)
	{
		words[x] = p.get(x);
	}
	return *this;
};

word& phrase::operator()(size_t x) const
{
	return words[x];
};

word& phrase::get(size_t x) const
{
	return words[x];
};

void phrase::print()
{
	for (size_t x = 0; x < n; x++){
		get(x).print();
	}
};