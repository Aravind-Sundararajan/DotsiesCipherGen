#ifndef TABLE_H_
#define TABLE_H_
#include "base.h"
#include "kvp.h"

class table
{
	private:
		const int TABLE_SIZE = 128;
		kvp ** t;
		bitset<5> ERROR = bitset<5>(33);
		
	public:
		//default constructor
		table()
		{
			t = new kvp*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++){
                  t[i] = NULL;
			}
		}
		
		//destructor
		~table()
		{
			for (int i = 0; i < TABLE_SIZE; i++){
                  if (t[i] != NULL){
                        delete t[i];
				  }
			}		
            delete[] t;
		}
		
		//copy constructor
		table(const table& tab)
		{
			t = tab.t;
		}
		
		//copy assignment
		table& operator=(const table& t)
		{
			return *this;
		}
		
		bitset<5> get(char key)
		{
			int k = hash(key);
			int h = (k % TABLE_SIZE);
			while (t[h] != NULL && t[h]->key != key)
				h = (h+1) % TABLE_SIZE;
			if (t[h] == NULL){
				return ERROR;
			}
			else {
				return t[h]->value;
			}
		}
		
		void put(char key,bitset<5> val)
		{
			int k = hash(key);
			int h = (k % TABLE_SIZE);
			while (t[h] != NULL && t[h]->key != key){
				h = (h+1) % TABLE_SIZE;
			}
			if (t[h] != NULL){
				delete t[h];
			}
			t[h] = new kvp(key, val);
		}
		
		int hash(char c)
		{
			int hash = (int)c;	
			//std::cout << "hash:" << hash << std::endl;
			return hash;
			
		}
		
		void print(){
			
		}
};

#endif