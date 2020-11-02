#ifndef TABLE_H_
#define TABLE_H_
#include "base.h"
#include "kvp.h"
using namespace std;
class table
{
	private:
		const int TABLE_SIZE = 128;
		kvp ** t;
		bitset<5> ERROR = bitset<5>(33);
		
	public:
		//default constructor
		table();
		
		//destructor
		~table();
		
		//copy constructor
		table(const table& tab);
		
		//copy assignment
		table& operator=(const table& t);
		
		bitset<5> get(char key);
		
		void put(char key,bitset<5> val);
		
		int hash(char c);
		
		void print();
};

#endif