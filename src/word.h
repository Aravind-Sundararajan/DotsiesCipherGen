#ifndef WORD_H_
#define WORD_H_
#include "base.h"
#include "letter.h"

using namespace std;
class word
{
	public:
		letter* letters{};
		size_t n = 0;
		
		//default constructor
		word();

		word(const letter l);
		
		//destructor
		~word();

		//copy constructor
		word(const word& w);

		//assignment copy
		word& operator=(const word& w);

		letter& operator()(size_t x) const;

		letter& get(size_t x) const;

		void print();

};

#endif
