#ifndef TABLE_H_
#define TABLE_H_
#include "base.h"
#include "kvp.h"
using namespace std;
class table
{
	private:
		const int TABLE_SIZE = 128; //
		kvp ** t;
		bitset<5> ERROR = bitset<5>(33);
		char ERRORC = '#';
	public:
		//default constructor
		table();

		//destructor
		~table();

		//copy constructor
		table(const table& tab);

		//copy assignment
		table& operator=(const table& t);

		//get bitset based on the character input
		bitset<5> get(char key);

		//inverse operation, get the character from the bitset
		char get(bitset<5> value);

		//put - put a key value pair in the table
		void put(char key,bitset<5> val);

		//"hashing" functions (let's think of a smarter way to do this...)
		int hash(char c);

		int hash(bitset<5> b);

		//print functions
		void print();
};

#endif
