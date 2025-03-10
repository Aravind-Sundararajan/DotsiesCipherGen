#ifndef LET_H_
#define LET_H_
#include "base.h"
using namespace std;

class letter
{
	public:
		bitset<5> b;
		//destructor
		~letter();

		letter();

		letter(const bitset<5> bin);

		//copy constructor
		letter(const letter& l);


		//copy assignment constructor
		letter& operator=(const letter& l);

		//getters
		bool operator()(size_t x) const;

		bool get(size_t x) const;

		//setters
		void set(size_t x, bool v);

		//print as bool array
		void printb() const;

		//circle_shift entire bitset by direction and position
		void circle_shift();
		void circle_shift(bool dir);
		void circle_shift(bool dir, int amount);

		//flip entire bitset
		void flip();

		//swap 2 values in bitset
		void swap(size_t p1, size_t p2);

};
#endif
