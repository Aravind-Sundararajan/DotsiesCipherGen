#ifndef LET_H_
#define LET_H_
#include "base.h"
using namespace std;

class letter
{
	public:
		bitset<5> b;		
		//destructor
		~letter()
		{
		};

		letter()
		{
			for (int x =0; x < 5; x++)
				b[x] = true;
		}
		
		letter(const bitset<5> bin)
		{
			for (int x =0; x < 5; x++)
				b[x] = bin[x];
		}
		
		//copy constructor
		letter(const letter& l)
		{
			for (int x = 0; x < 5; x++)
				b[x] = l.b[x];
		};


		//copy assignment constructor
		letter& operator=(const letter& l)
		{
			for (int x = 0; x < 5; x++)
				b[x] = l.b[x];


			return *this;
		};

		//getters
		bool operator()(size_t x) const
		{
			return b[x];
		}

		bool get(size_t x) const
		{
			return b[x];
		}

		//print as bool array
		void printb()
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

};
#endif
