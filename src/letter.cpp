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
};

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

//setters
void letter::set(size_t x, bool v)
{
	b[x] = v;
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

void letter::circle_shift()
{
		bool start = b[0];
		for (int x = 0; x < 5; x++)
			b.set(x % 5,b[(x + 1) % 5]);
		b.set(4,start);
}

void letter::circle_shift(bool dir)
{
	if (dir){
		bool start = b[0];
		for (int x = 0; x < 5; x++)
			b.set(x % 5,b[(x + 1) % 5]);
		b.set(4,start);
	}else{
		bool start = b[4];
		for (int x = 4; x > -1; x--)
			b.set(x % 5,b[(x - 1) % 5]);
		b.set(0,start);
	}
}

void letter::circle_shift(bool dir, int amount)
{
	for (int x =0; x < amount; x++){
			circle_shift(dir);
	}
}

void letter::flip()
{
	b = b.flip();
}

void letter::swap(std::size_t p1, std::size_t p2)
{
	bool temp;
	temp = b[p1];
	b.set(p1,b[p2]);
	b.set(p2,temp);
}
