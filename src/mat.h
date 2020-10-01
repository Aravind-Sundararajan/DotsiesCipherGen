#ifndef MAT_H_
#define MAT_H_
#include "base.h"
#include "letter.h"
#include "word.h"
#include "phrase.h"

using namespace std;
class mat
{
	public:
		bool* elem[] {};
		int n = 0;
		
		//default constructor
		mat()
		{
			elem = new bool[5];
			n=1;
		}
		
		//destructor
		~mat()
		{
			delete[] elem;
		}
		
		//copy constructor
		mat(const mat& m)
		{
			n = m.n;
			elem = new bool [5*n]{};
			for (size_t x =0; x < n; x++)
			{
				elem[x] = m.get(x);
			}
		}

		//phrase constructor
		mat(const phrase& p)
		{
			n = p.n;
			elem = new bool [5*n]{};
			for (size_t x =0; x < n; x++)
			{
				for (size_t y =0; y < 5; y++)
				{
					elem[5*x + y] = (p.get(x)).get(y);
				}
			}
		}
		
		//copy assignment
		mat& operator=(const mat& m)
		{
			delete[] elem;
			n = m.n;
			elem = new bool [5*n]{};
			for (size_t x =0; x < n; x++)
			{
				elem[x] = m.get(x);
			}
			return *this;
		}
};


#endif