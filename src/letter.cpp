#include "letter.h"
using namespace std;

//destructor
letter::~letter(){};

letter::letter()
{
	for (int x =0; x < 5; x++)
		b[x] = true;
}

letter::letter(const bitset<5> bin)
{
	for (int x =0; x < 5; x++)
		b[x] = bin[x];
}

//copy constructor
letter::letter(const letter& l)
{
	for (int x = 0; x < 5; x++)
		b[x] = l.b[x];
};


//copy assignment constructor
letter& letter::operator=(const letter& l)
{
	for (int x = 0; x < 5; x++)
		b[x] = l.b[x];


	return *this;
};

//getters
bool letter::operator()(size_t x) const
{
	return b[x];
}

bool letter::get(size_t x) const
{
	return b[x];
}

//print as bool array
void letter::printb()
{
	//cout << "printing letter:" << endl;
	for (int x = 0; x < 5; x++){
		if (b.test(x)){
			cout << "#";
		}else{
			cout << " ";
		}
	}
	cout << endl;
};