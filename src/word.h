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
		word()
		{
			letters = new letter[1]{};
			n = 1;
		};

		//destructor
		~word()
		{
			delete[] letters;
		};

		//copy constructor
		word(const word& w)
		{
			n = w.n;
			letters = new letter [n]{};
			for (size_t x =0; x < n; x++)
			{
				letters[x] = w.get(x);
			}
		}

		//assignment copy
		word& operator=(const word& w)
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

		letter& operator()(size_t x) const
		{
			return letters[x];
		}

		letter& get(size_t x) const
		{
			return letters[x];
		}

		void print()
		{
			for (size_t x = 0; x < n; x++){
				get(x).printb();
			}
		}



};

#endif
