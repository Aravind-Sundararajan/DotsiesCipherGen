#ifndef PHRASE_H_
#define PHRASE_H_
#include "base.h"
#include "letter.h"
#include "word.h"
using namespace std;
class phrase
{
	public:
		word* words{};
		size_t n = 0;
		//default constructor
		phrase();
		
		phrase(const word w);

		//destructor
		~phrase();

		//copy constructor
		phrase(const phrase& p);

		//assignment copy
		phrase& operator=(const phrase& p);

		word& operator()(size_t x) const;

		word& get(size_t x) const;

		void print();
};

#endif
