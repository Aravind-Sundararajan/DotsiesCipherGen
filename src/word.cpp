#include "word.h"
using namespace std;

//default constructor
word::word()
{
	letters = new letter[1]{};
	n = 1;
};

word::word(const letter l)
{
	letters = new letter[1]{};
	n = 1;
	letters[0] = l;
};

//destructor
word::~word()
{
	delete[] letters;
};

//copy constructor
word::word(const word& w)
{
	n = w.n;
	letters = new letter [n]{};
	for (size_t x =0; x < n; x++)
	{
		letters[x] = w.get(x);
	}
};

//assignment copy
word& word::operator=(const word& w)
{
	delete[] letters;
	n = w.n;
	letters = new letter[n]{};
	for (size_t x = 0; x < n; x++)
	{
		letters[x] = w.get(x);
	}
	return *this;
};

letter& word::operator()(size_t x) const
{
	return letters[x];
};

letter& word::get(size_t x) const
{
	return letters[x];
};

void word::print()
{
	for (size_t x = 0; x < n; x++){
		get(x).printb();
	}
};