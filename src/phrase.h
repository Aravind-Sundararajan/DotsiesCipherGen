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
		phrase()
		{
			words = new word[1]{};
			n = 1;
		};

		//destructor
		~phrase()
		{
			delete[] words;
		};

		//copy constructor
		phrase(const phrase& p)
		{
			n = p.n;
			words = new word [n]{};
			for (size_t x =0; x < n; x++)
			{
				words[x] = p.get(x);
			}
		}

		//assignment copy
		phrase& operator=(const phrase& p)
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

		word& operator()(size_t x) const
		{
			return words[x];
		}

		word& get(size_t x) const
		{
			return words[x];
		}

		void print()
		{
			for (size_t x = 0; x < n; x++){
				get(x).print();
			}
		}



};

#endif
