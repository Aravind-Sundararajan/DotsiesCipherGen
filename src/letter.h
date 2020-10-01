#ifndef LET_H_
#define LET_H_
#include "base.h"
using namespace std;

class letter
{
	public:
		bool b[5];		
		//destructor
		~letter()
		{
		};

		letter()
		{
			for (int x =0; x < 5; x++)
				b[x] = true;
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

		//print as bool array
		void printb()
		{
			cout << "printing letter:" << endl;
			for (int x = 0; x < 5; x++){
				if (b[x]){
					cout << "#";
				}else{
					cout << " ";
				}
			}
			cout << endl;
		};

};
#endif
