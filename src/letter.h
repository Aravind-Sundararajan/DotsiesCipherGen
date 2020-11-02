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

		//print as bool array
		void printb();

};
#endif
